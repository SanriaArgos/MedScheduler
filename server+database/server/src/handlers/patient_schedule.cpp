#include "../../include/handlers/patient_schedule.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void view_doctor_schedule_for_patient(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json response;

    // Проверка наличия необходимых полей
    if (!data.contains("hospital_id") || !data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing required fields (hospital_id, doctor_id)";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
 
    // Извлечение параметров
    int hospital_id = data["hospital_id"];
    int doctor_id = data["doctor_id"];

    std::string doctor_id_str = std::to_string(doctor_id);
    std::string hospital_id_str = std::to_string(hospital_id);

    const char *params[2] = {doctor_id_str.c_str(), hospital_id_str.c_str()};

    PGresult *res_query = PQexecParams(
        db_handler.get_connection(),
        R"(
          SELECT appointment_date, appointment_time, cabinet_number, patient_id
          FROM records
           WHERE doctor_id   = $1::int
             AND hospital_id = $2::int
             AND appointment_date BETWEEN CURRENT_DATE
                                     AND (CURRENT_DATE + INTERVAL '7 day')
          ORDER BY appointment_date, appointment_time
        )",
        2, nullptr, params, nullptr, nullptr, 0
    );

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] = "Schedule not available";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        if (res_query) {
            PQclear(res_query);
        }
        return;
    }

    int rows = PQntuples(res_query);
    json schedule = json::array();
    for (int i = 0; i < rows; ++i) {
        json record;
        record["appointment_date"] = PQgetvalue(res_query, i, 0);
        record["appointment_time"] = PQgetvalue(res_query, i, 1);
        record["cabinet_number"] = PQgetvalue(res_query, i, 2);
        record["patient_id"] = PQgetvalue(res_query, i, 3);
        schedule.push_back(record);
    }
    PQclear(res_query);

    response["success"] = true;
    response["schedule"] = schedule;

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}