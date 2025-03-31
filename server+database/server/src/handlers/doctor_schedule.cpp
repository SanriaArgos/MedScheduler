#include "../../include/handlers/doctor_schedule.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void display_doctor_schedule(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json response;

    // Проверка на наличие обязательных полей
    if (!data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing doctor_id";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int doctor_id = data["doctor_id"];
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params[1] = { doctor_id_str.c_str() };
    PGresult *res_query = PQexecParams(db_handler.get_connection(),
        "SELECT r.appointment_date, r.appointment_time, h.full_name, r.cabinet_number, "
        "u.last_name, u.first_name, u.patronymic, u.phone "
        "FROM records r "
        "JOIN hospitals h ON r.hospital_id = h.hospital_id "
        "LEFT JOIN users u ON r.patient_id = u.id "
        "WHERE r.doctor_id = $1 "
        "AND r.appointment_date BETWEEN CURRENT_DATE AND (CURRENT_DATE + INTERVAL '7 day') "
        "ORDER BY r.appointment_date, r.appointment_time",
        1, nullptr, params, nullptr, nullptr, 0);

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] = "Schedule not available";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        if (res_query) PQclear(res_query);
        return;
    }

    int rows = PQntuples(res_query);
    json schedule = json::array();
    for (int i = 0; i < rows; ++i) {
        json record;
        record["appointment_date"] = PQgetvalue(res_query, i, 0);
        record["appointment_time"] = PQgetvalue(res_query, i, 1);
        record["hospital_full_name"] = PQgetvalue(res_query, i, 2);
        record["cabinet_number"] = PQgetvalue(res_query, i, 3);
        record["patient_last_name"] = PQgetvalue(res_query, i, 4);
        record["patient_first_name"] = PQgetvalue(res_query, i, 5);
        record["patient_patronymic"] = PQgetvalue(res_query, i, 6);
        record["patient_phone"] = PQgetvalue(res_query, i, 7);
        schedule.push_back(record);
    }

    PQclear(res_query);

    response["success"] = true;
    response["schedule"] = schedule;

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}