#include "../../include/handlers/get_doctor_schedule_for_patient.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void get_doctor_schedule_for_patient(
    int doctor_id,
    int hospital_id,
    http::response<http::string_body>& res,
    database_handler& db_handler
) {
    json response;

    // Подготовка параметров для SQL
    std::string doctor_id_str   = std::to_string(doctor_id);
    std::string hospital_id_str = std::to_string(hospital_id);
    const char* params[2]       = {doctor_id_str.c_str(), hospital_id_str.c_str()};

    // Выполнение запроса
    PGresult* pgres = PQexecParams(
        db_handler.get_connection(),
        R"(
            SELECT appointment_date,
                   appointment_time,
                   cabinet_number,
                   patient_id
              FROM records
             WHERE doctor_id   = $1::int
               AND hospital_id = $2::int
               AND appointment_date BETWEEN CURRENT_DATE
                                       AND (CURRENT_DATE + INTERVAL '7 day')
             ORDER BY appointment_date, appointment_time
        )",
        2,        // количество параметров
        nullptr,  // разрешить PG автоматически определять типы
        params,
        nullptr,  // длины параметров по умолчанию
        nullptr,  // форматы параметров по умолчанию
        0         // текстовый вывод
    );

    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"]   = "Schedule not available";
        if (pgres) PQclear(pgres);

        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Собираем результат
    int rows = PQntuples(pgres);
    json schedule = json::array();
    for (int i = 0; i < rows; ++i) {
        schedule.push_back({
            {"appointment_date",  PQgetvalue(pgres, i, 0)},
            {"appointment_time",  PQgetvalue(pgres, i, 1)},
            {"cabinet_number",    PQgetvalue(pgres, i, 2)},
            {"patient_id",        PQgetvalue(pgres, i, 3)}
        });
    }
    PQclear(pgres);

    // Отправляем ответ
    response["success"]  = true;
    response["schedule"] = schedule;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
