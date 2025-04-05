#include "../../include/database.hpp"
#include "../../include/handlers/doctor_hospital_managment.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <algorithm>
#include <regex>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

static bool get_junior_admin_hospital_id(int junior_admin_id, int &out_hospital_id) {
    std::string admin_id_str = std::to_string(junior_admin_id);
    const char* params[1] = { admin_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT hospital_id FROM hospitals WHERE administrator_id = $1 LIMIT 1",
        1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        out_hospital_id = std::stoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        return true;
    }
    PQclear(res);
    std::cerr << "Error: Could not retrieve hospital for junior administrator\n";
    return false;
}

void add_hospital_to_doctor(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json response;

    // Проверка на наличие обязательных полей
    if (!data.contains("doctor_id") || !data.contains("hospital_id") || !data.contains("junior_admin_id")) {
        std::cerr << "Error: Missing required fields for add_hospital_to_doctor\n";
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int doctor_id = data["doctor_id"];
    int hospital_id = data["hospital_id"];
    int junior_admin_id = data["junior_admin_id"];
    
    // Проверяем, что врач существует.
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params_doc[1] = { doctor_id_str.c_str() };
    PGresult *res_doc = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1",
        1, NULL, params_doc, NULL, NULL, 0);
    if (!(PQresultStatus(res_doc) == PGRES_TUPLES_OK && PQntuples(res_doc) > 0)) {
        std::cerr << "Error: Doctor not found\n";
        response["success"] = false;
        response["error"] = "Doctor not found";

        res.result(http::status::not_found);  // 404 Not Found
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        if (res_doc) PQclear(res_doc);
        return;
    }
    PQclear(res_doc);

    // Проверяем, что заданная больница принадлежит данному младшему администратору.
    std::string hospital_id_str = std::to_string(hospital_id);
    std::string junior_admin_id_str = std::to_string(junior_admin_id);
    const char* params_hosp[2] = { hospital_id_str.c_str(), junior_admin_id_str.c_str() };
    PGresult *res_hosp = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = $2",
        2, NULL, params_hosp, NULL, NULL, 0);
    if (!(PQresultStatus(res_hosp) == PGRES_TUPLES_OK && PQntuples(res_hosp) > 0)) {
        std::cerr << "Error: Hospital ID does not match your junior administrator\n";
        response["success"] = false;
        response["error"] = "Hospital ID does not match your junior administrator";

        res.result(http::status::forbidden);  // 403 Forbidden
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_hosp);
        return;
    }
    PQclear(res_hosp);

    // Проверяем, что данная больница ещё не добавлена в список врача.
    const char* params_check[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_check = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check, NULL, NULL, 0);
    if (PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0) {
        std::cerr << "Error: Hospital ID already exists in doctor's list\n";
        response["success"] = false;
        response["error"] = "Hospital ID already exists in doctor's list";

        res.result(http::status::conflict);  // 409 Conflict
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_check);
        return;
    }
    PQclear(res_check);

    const char* params_update[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_update = PQexecParams(db_handler.get_connection(),
        "UPDATE doctors SET hospital_ids = array_append(hospital_ids, $2) WHERE doctor_id = $1",
        2, NULL, params_update, NULL, NULL, 0);
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding Hospital ID: " << PQerrorMessage(db_handler.get_connection()) << "\n";
        response["success"] = false;
        response["error"] = "Error adding Hospital ID";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_update);
        return;
    }
    PQclear(res_update);

    std::cerr << "Hospital ID added to doctor's list\n";
    response["success"] = true;
    response["message"] = "Hospital ID added to doctor's list";

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}

void remove_hospital_from_doctor(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json response;

    // Проверка на наличие обязательных полей
    if (!data.contains("doctor_id") || !data.contains("hospital_id") || !data.contains("junior_admin_id")) {
        std::cerr << "Error: Missing required fields for remove_hospital_from_doctor\n";
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int doctor_id = data["doctor_id"];
    int hospital_id = data["hospital_id"];
    int junior_admin_id = data["junior_admin_id"];
    
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params_doc[1] = { doctor_id_str.c_str() };
    PGresult *res_doc = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1",
        1, NULL, params_doc, NULL, NULL, 0);
    if (!(PQresultStatus(res_doc) == PGRES_TUPLES_OK && PQntuples(res_doc) > 0)) {
        std::cerr << "Error: Doctor not found\n";
        response["success"] = false;
        response["error"] = "Doctor not found";

        res.result(http::status::not_found);  // 404 Not Found
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        if (res_doc) PQclear(res_doc);
        return;
    }
    PQclear(res_doc);
    
    std::string hospital_id_str = std::to_string(hospital_id);
    std::string junior_admin_id_str = std::to_string(junior_admin_id);
    const char* params_hosp[2] = { hospital_id_str.c_str(), junior_admin_id_str.c_str() };
    PGresult *res_hosp = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = $2",
        2, NULL, params_hosp, NULL, NULL, 0);
    if (!(PQresultStatus(res_hosp) == PGRES_TUPLES_OK && PQntuples(res_hosp) > 0)) {
        std::cerr << "Error: Hospital ID does not match your junior administrator\n";
        response["success"] = false;
        response["error"] = "Hospital ID does not match your junior administrator";

        res.result(http::status::forbidden);  // 403 Forbidden
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

        PQclear(res_hosp);
        return;
    }
    PQclear(res_hosp);
    
    const char* params_check[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_check = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check, NULL, NULL, 0);
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0)) {
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
    
    const char* params_update[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_update = PQexecParams(db_handler.get_connection(),
        "UPDATE doctors SET hospital_ids = array_remove(hospital_ids, $2) WHERE doctor_id = $1",
        2, NULL, params_update, NULL, NULL, 0);
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
        std::cerr << "Error removing Hospital ID: " << PQerrorMessage(db_handler.get_connection()) << "\n";
        response["success"] = false;
        response["error"] = "Error removing Hospital ID";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
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