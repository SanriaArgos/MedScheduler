#include "../../include/handlers/junior_admin_schedule.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include "../../include/handlers/add_record_slot.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler *global_db;

void junior_admin_schedule(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // Проверка на наличие обязательных полей
    if (!data.contains("junior_admin_id") || !data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing junior_admin_id or doctor_id";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int junior_admin_id = data["junior_admin_id"];
    int doctor_id = data["doctor_id"];

    int my_hospital_id;
    if (!get_junior_admin_hospital_id(junior_admin_id)) {
        response["success"] = false;
        response["error"] = "Your hospital not found";

        res.result(http::status::not_found);  // 404 Not Found
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string doctor_id_str = std::to_string(doctor_id);
    std::string hospital_id_str = std::to_string(my_hospital_id);
    const char *params[2] = {doctor_id_str.c_str(), hospital_id_str.c_str()};
    PGresult *res_query = PQexecParams(
        db_handler.get_connection(),
        "SELECT r.record_id, r.appointment_date, r.appointment_time, "
        "r.cabinet_number, r.patient_id, d.price "
        "FROM records r "
        "JOIN doctors d ON r.doctor_id = d.doctor_id "
        "WHERE r.doctor_id = $1 AND r.hospital_id = $2 "
        "AND r.appointment_date BETWEEN CURRENT_DATE "
        "AND (CURRENT_DATE + INTERVAL '7 day') "
        "ORDER BY r.appointment_date, r.appointment_time",
        2, nullptr, params, nullptr, nullptr, 0
    );

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] = "Schedule not available";

        res.result(http::status::internal_server_error);
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
        record["record_id"] = PQgetvalue(res_query, i, 0);
        record["appointment_date"] = PQgetvalue(res_query, i, 1);
        record["appointment_time"] = PQgetvalue(res_query, i, 2);
        record["cabinet_number"] = PQgetvalue(res_query, i, 3);
        record["patient_id"] = PQgetvalue(res_query, i, 4);
        record["price"] = PQgetvalue(res_query, i, 5);
        schedule.push_back(record);
    }

    PQclear(res_query);

    response["success"] = true;
    response["schedule"] = schedule;

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}