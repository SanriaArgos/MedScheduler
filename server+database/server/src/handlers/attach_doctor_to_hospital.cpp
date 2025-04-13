#include "../../include/handlers/attach_doctor_to_hospital.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void attach_doctor_to_hospital(
    const json &body,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json result;

    int doctor_id = body.value("doctor_id", -1);
    int hospital_id = body.value("hospital_id", -1);

    if (doctor_id == -1 || hospital_id == -1) {
        result = {
            {"success", false},
            {"error", "Missing or invalid doctor_id or hospital_id"}};
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = result.dump();
        return;
    }

    PGconn *conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        result = {{"success", false}, {"error", "Database connection failed"}};
        res.result(http::status::service_unavailable);
        res.set(http::field::content_type, "application/json");
        res.body() = result.dump();
        return;
    }

    // // Проверяем, есть ли уже hospital_id в hospital_ids у данного доктора
    // const char* check_params[1] = {std::to_string(doctor_id).c_str()};
    // PGresult* check_res = PQexecParams(conn,
    //     "SELECT hospital_ids FROM doctors WHERE doctor_id = $1",
    //     1, nullptr, check_params, nullptr, nullptr, 0);

    // if (!check_res || PQresultStatus(check_res) != PGRES_TUPLES_OK) {
    //     result = {
    //         {"success", false},
    //         {"error", "Failed to check existing hospitals"}
    //     };
    //     res.result(http::status::internal_server_error);
    //     PQclear(check_res);
    //     res.set(http::field::content_type, "application/json");
    //     res.body() = result.dump();
    //     return;
    // }

    // std::string hospital_ids_str = PQgetvalue(check_res, 0, 0);
    // PQclear(check_res);

    // // Проверяем, есть ли уже hospital_id в списке hospital_ids
    // if (hospital_ids_str.find(std::to_string(hospital_id)) !=
    // std::string::npos) {
    //     result = {
    //         {"success", false},
    //         {"error", "Doctor is already attached to this hospital"}
    //     };
    //     res.result(http::status::conflict);
    //     res.set(http::field::content_type, "application/json");
    //     res.body() = result.dump();
    //     return;
    // }

    // Добавляем hospital_id в hospital_ids
    const char *update_params[2] = {
        std::to_string(doctor_id).c_str(), std::to_string(hospital_id).c_str()};
    PGresult *update_res = PQexecParams(
        conn,
        "UPDATE doctors SET hospital_ids = array_append(COALESCE(hospital_ids, "
        "'{}'::integer[]), $2::integer) WHERE doctor_id = $1",
        2, nullptr, update_params, nullptr, nullptr, 0
    );
    if (!update_res || PQresultStatus(update_res) != PGRES_COMMAND_OK) {
        result = {
            {"success", false},
            {"error", "Failed to update doctor's hospitals"}};
        res.result(http::status::internal_server_error);
    } else {
        result = {
            {"success", true},
            {"message", "Doctor successfully attached to hospital"}};
        res.result(http::status::ok);
    }

    PQclear(update_res);
    res.set(http::field::content_type, "application/json");
    res.body() = result.dump();
}
