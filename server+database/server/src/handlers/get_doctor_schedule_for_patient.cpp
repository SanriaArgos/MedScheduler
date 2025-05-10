#include "../../include/handlers/get_doctor_schedule_for_patient.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler *global_db;

void get_doctor_schedule_for_patient(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // Подготовка параметра для SQL
    std::string doctor_id_str = std::to_string(doctor_id);
    const char *params[1] = {doctor_id_str.c_str()};

    // Модифицированный запрос с добавлением hospital_id в выборку
    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        R"(
          SELECT
            r.appointment_date,
            r.appointment_time,
            h.region,
            h.settlement_type,
            h.settlement_name,
            h.street,
            h.house,
            h.full_name,
            u.phone AS junior_admin_phone,
            CASE WHEN r.patient_id IS NULL THEN 'open' ELSE 'close' END AS slot_status,
            h.hospital_id
          FROM records r
          JOIN hospitals h ON r.hospital_id = h.hospital_id
          JOIN users u ON h.administrator_id = u.id
          WHERE r.doctor_id = $1::int
          ORDER BY r.appointment_date, r.appointment_time
        )",
        1,        // только 1 параметр (doctor_id)
        nullptr,  // типы параметров
        params,
        nullptr,  // длины параметров
        nullptr,  // форматы параметров
        0         // текстовый вывод
    );

    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] = "Schedule not available";
        if (pgres) {
            PQclear(pgres);
        }

        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Собираем результат в том же формате, но с добавлением hospital_id
    int rows = PQntuples(pgres);
    json schedule = json::array();
    for (int i = 0; i < rows; ++i) {
        json slot;
        slot["appointment_date"] = PQgetvalue(pgres, i, 0);
        slot["appointment_time"] = PQgetvalue(pgres, i, 1);
        slot["region"] = PQgetvalue(pgres, i, 2);
        slot["settlement_type"] = PQgetvalue(pgres, i, 3);
        slot["settlement_name"] = PQgetvalue(pgres, i, 4);
        slot["street"] = PQgetvalue(pgres, i, 5);
        slot["house"] = PQgetvalue(pgres, i, 6);
        slot["full_name"] = PQgetvalue(pgres, i, 7);
        slot["junior_admin_phone"] = PQgetvalue(pgres, i, 8);
        slot["slot_status"] = PQgetvalue(pgres, i, 9);
        slot["hospital_id"] = PQgetvalue(pgres, i, 10);  // Добавляем hospital_id
        
        schedule.push_back(std::move(slot));
    }
    PQclear(pgres);

    // Отправляем ответ в том же формате, но с hospital_id
    response["success"] = true;
    response["schedule"] = schedule;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}