#include "../../include/database.hpp"
#include "../../include/handlers/doctor_hospital_managment.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <algorithm>
#include <regex>
#include <string>
#include <nlohmann/json.hpp>
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

bool add_hospital_to_doctor(const json &data) {
    if (!data.contains("doctor_id") || !data.contains("hospital_id") || !data.contains("junior_admin_id")) {
        std::cerr << "Error: Missing required fields for add_hospital_to_doctor\n";
        return false;
    }
    int doctor_id = data["doctor_id"];
    int hospital_id = data["hospital_id"];
    int junior_admin_id = data["junior_admin_id"];
    
    // Проверяем, что врач существует.
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params_doc[1] = { doctor_id_str.c_str() };
    PGresult *res_doc = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1",
        1, NULL, params_doc, NULL, NULL, 0);
    if (!(PQresultStatus(res_doc) == PGRES_TUPLES_OK && PQntuples(res_doc) > 0)) {
        std::cerr << "Error: Doctor not found\n";
        if (res_doc) PQclear(res_doc);
        return false;
    }
    PQclear(res_doc);

    // Проверяем, что заданная больница принадлежит данному младшему администратору.
    std::string hospital_id_str = std::to_string(hospital_id);
    std::string junior_admin_id_str = std::to_string(junior_admin_id);
    const char* params_hosp[2] = { hospital_id_str.c_str(), junior_admin_id_str.c_str() };
    PGresult *res_hosp = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = $2",
        2, NULL, params_hosp, NULL, NULL, 0);
    if (!(PQresultStatus(res_hosp) == PGRES_TUPLES_OK && PQntuples(res_hosp) > 0)) {
        std::cerr << "Error: Hospital ID does not match your junior administrator\n";
        PQclear(res_hosp);
        return false;
    }
    PQclear(res_hosp);

    // Проверяем, что данная больница ещё не добавлена в список врача.
    const char* params_check[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_check = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check, NULL, NULL, 0);
    if (PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0) {
        std::cerr << "Error: Hospital ID already exists in doctor's list\n";
        PQclear(res_check);
        return false;
    }
    PQclear(res_check);

    const char* params_update[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_update = PQexecParams(global_db->get_connection(),
        "UPDATE doctors SET hospital_ids = array_append(hospital_ids, $2) WHERE doctor_id = $1",
        2, NULL, params_update, NULL, NULL, 0);
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding Hospital ID: " << PQerrorMessage(global_db->get_connection()) << "\n";
        PQclear(res_update);
        return false;
    }
    PQclear(res_update);
    std::cerr << "Hospital ID added to doctor's list\n";
    return true;
}

bool remove_hospital_from_doctor(const json &data) {
    if (!data.contains("doctor_id") || !data.contains("hospital_id") || !data.contains("junior_admin_id")) {
        std::cerr << "Error: Missing required fields for remove_hospital_from_doctor\n";
        return false;
    }
    int doctor_id = data["doctor_id"];
    int hospital_id = data["hospital_id"];
    int junior_admin_id = data["junior_admin_id"];
    
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params_doc[1] = { doctor_id_str.c_str() };
    PGresult *res_doc = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1",
        1, NULL, params_doc, NULL, NULL, 0);
    if (!(PQresultStatus(res_doc) == PGRES_TUPLES_OK && PQntuples(res_doc) > 0)) {
        std::cerr << "Error: Doctor not found\n";
        if (res_doc) PQclear(res_doc);
        return false;
    }
    PQclear(res_doc);
    
    std::string hospital_id_str = std::to_string(hospital_id);
    std::string junior_admin_id_str = std::to_string(junior_admin_id);
    const char* params_hosp[2] = { hospital_id_str.c_str(), junior_admin_id_str.c_str() };
    PGresult *res_hosp = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = $2",
        2, NULL, params_hosp, NULL, NULL, 0);
    if (!(PQresultStatus(res_hosp) == PGRES_TUPLES_OK && PQntuples(res_hosp) > 0)) {
        std::cerr << "Error: Hospital ID does not match your junior administrator\n";
        PQclear(res_hosp);
        return false;
    }
    PQclear(res_hosp);
    
    const char* params_check[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_check = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check, NULL, NULL, 0);
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0)) {
        std::cerr << "Error: Hospital ID not found in doctor's list\n";
        PQclear(res_check);
        return false;
    }
    PQclear(res_check);
    
    const char* params_update[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res_update = PQexecParams(global_db->get_connection(),
        "UPDATE doctors SET hospital_ids = array_remove(hospital_ids, $2) WHERE doctor_id = $1",
        2, NULL, params_update, NULL, NULL, 0);
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
        std::cerr << "Error removing Hospital ID: " << PQerrorMessage(global_db->get_connection()) << "\n";
        PQclear(res_update);
        return false;
    }
    PQclear(res_update);
    std::cerr << "Hospital ID removed from doctor's list\n";
    return true;
}