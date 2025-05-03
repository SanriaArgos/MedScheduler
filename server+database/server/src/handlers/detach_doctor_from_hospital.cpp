#include "../../include/handlers/detach_doctor_from_hospital.hpp"
#include <libpq-fe.h>
#include <algorithm>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>
#include <string>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void detach_doctor_from_hospital(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    std::cerr << "DEBUG detach received JSON: " 
          << data.dump() << std::endl;

    json response;

    if (!data.contains("doctor_id") || !data.contains("hospital_id") ||
        !data.contains("junior_admin_id")) {
        std::cerr << "Error: Missing required fields for "
                     "detach_doctor_from_hospital\n";
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::cerr << 193 << "" << "\n";
    int doctor_id = data["doctor_id"];
    int hospital_id = data["hospital_id"];
    int junior_admin_id = data["junior_admin_id"];

    std::string doctor_id_str = std::to_string(doctor_id);
    const char *params_doc[1] = {doctor_id_str.c_str()};
    PGresult *res_doc = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1", 1, NULL, params_doc, NULL,
        NULL, 0
    );

    if (!(PQresultStatus(res_doc) == PGRES_TUPLES_OK && PQntuples(res_doc) > 0
        )) {
        std::cerr << "Error: Doctor not found\n";
        response["success"] = false;
        response["error"] = "Doctor not found";

        res.result(http::status::not_found);  // 404 Not Found
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        if (res_doc) {
            PQclear(res_doc);
        }
        return;
    }
    PQclear(res_doc);

    std::string hospital_id_str = std::to_string(hospital_id);
    std::string junior_admin_id_str = std::to_string(junior_admin_id);
    const char *params_hosp[2] = {
        hospital_id_str.c_str(), junior_admin_id_str.c_str()};
    PGresult *res_hosp = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = "
        "$2",
        2, NULL, params_hosp, NULL, NULL, 0
    );

    if (!(PQresultStatus(res_hosp) == PGRES_TUPLES_OK && PQntuples(res_hosp) > 0
        )) {
        std::cerr
            << "Error: Hospital ID does not match your junior administrator\n";
        response["success"] = false;
        response["error"] =
            "Hospital ID does not match your junior administrator";

        res.result(http::status::forbidden);  // 403 Forbidden
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_hosp);
        return;
    }
    PQclear(res_hosp);

    const char *params_check[2] = {
        doctor_id_str.c_str(), hospital_id_str.c_str()};
    PGresult *res_check = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check, NULL, NULL, 0
    );
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK &&
          PQntuples(res_check) > 0)) {
        std::cerr << "Error: Hospital ID not found in doctor's list\n";
        response["success"] = false;
        response["error"] = "Hospital ID not found in doctor's list";

        res.result(http::status::not_found);  // 404 Not Found
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_check);
        return;
    }
    PQclear(res_check);

    std::cerr << 277 << "" << "\n";

    const char *params_update[2] = {
        doctor_id_str.c_str(), hospital_id_str.c_str()};
    PGresult *res_update = PQexecParams(
        db_handler.get_connection(),
        "UPDATE doctors SET hospital_ids = array_remove(hospital_ids, $2) "
        "WHERE doctor_id = $1",
        2, NULL, params_update, NULL, NULL, 0
    );
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
        std::cerr << "Error removing Hospital ID: "
                  << PQerrorMessage(db_handler.get_connection()) << "\n";
        response["success"] = false;
        response["error"] = "Error removing Hospital ID";

        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_update);
        return;
    }
    PQclear(res_update);


    std::cerr << "Hospital ID removed from doctor's list\n";
    response["success"] = true;
    response["message"] = "Hospital ID removed from doctor's list";

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}