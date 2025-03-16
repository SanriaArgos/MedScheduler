#include "doctor_hospital_management.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <algorithm>
#include <regex>

// Helper: get the hospital ID of the junior administrator (assumes one hospital per admin)
static int get_junior_admin_hospital_id(database_handler &db, int junior_admin_id) {
    int hospital_id = -1;
    std::stringstream ss;
    ss << "SELECT hospital_id FROM hospitals WHERE administrator_id = " << junior_admin_id << " LIMIT 1";
    PGresult *res = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        hospital_id = static_cast<int>(std::stoi(PQgetvalue(res, 0, 0)));
    }
    PQclear(res);
    return hospital_id;
}

void add_hospital_to_doctor(database_handler &db, int junior_admin_id) {
    std::cout << "\n=== Add Your Hospital ID to Doctor's List ===\n";
    int doctor_id;
    while (true) {
        std::string doctor_id_str = get_validated_input("Enter Doctor ID", true);
        try {
            doctor_id = static_cast<int>(std::stoi(doctor_id_str));
        } catch (...) {
            std::cout << "Error: Enter a number\n";
            continue;
        }
        std::string query_doc = "SELECT 1 FROM doctors WHERE doctor_id = " + std::to_string(doctor_id);
        PGresult *res = PQexec(db.get_connection(), query_doc.c_str());
        if (!(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)) {
            std::cout << "Error: Doctor not found\n";
            PQclear(res);
            continue;
        }
        PQclear(res);
        break;
    }
    int hosp_id;
    while (true) {
        std::string hospital_id_str = get_validated_input("Enter Hospital ID", true);
        try {
            hosp_id = static_cast<int>(std::stoi(hospital_id_str));
        } catch (...) {
            std::cout << "Error: Enter a number\n";
            continue;
        }
        std::string check_sql = "SELECT 1 FROM hospitals WHERE hospital_id = " + std::to_string(hosp_id)
                                + " AND administrator_id = " + std::to_string(junior_admin_id);
        PGresult *res = PQexec(db.get_connection(), check_sql.c_str());
        if (!(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)) {
            std::cout << "Error: Hospital ID does not match yours\n";
            PQclear(res);
            continue;
        }
        PQclear(res);
        break;
    }
    std::stringstream ss_check;
    ss_check << "SELECT 1 FROM doctors WHERE doctor_id = " << doctor_id
             << " AND " << hosp_id << " = ANY(hospital_ids)";
    PGresult *res_check = PQexec(db.get_connection(), ss_check.str().c_str());
    if (PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0) {
        std::cout << "Error: Hospital ID already exists in doctor's list\n";
        PQclear(res_check);
        return;
    }
    PQclear(res_check);
    std::stringstream ss;
    ss << "UPDATE doctors SET hospital_ids = array_append(hospital_ids, " << hosp_id 
       << ") WHERE doctor_id = " << doctor_id;
    PGresult *res = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        std::cout << "Hospital ID added to doctor's list\n";
    } else {
        std::cout << "Error adding Hospital ID\n";
    }
    PQclear(res);
}

void remove_hospital_from_doctor(database_handler &db, int junior_admin_id) {
    std::cout << "\n=== Remove Your Hospital ID from Doctor's List ===\n";
    int doctor_id;
    while (true) {
        std::string doctor_id_str = get_validated_input("Enter Doctor ID", true);
        try {
            doctor_id = static_cast<int>(std::stoi(doctor_id_str));
        } catch (...) {
            std::cout << "Error: Enter a number\n";
            continue;
        }
        std::string query_doc = "SELECT 1 FROM doctors WHERE doctor_id = " + std::to_string(doctor_id);
        PGresult *res = PQexec(db.get_connection(), query_doc.c_str());
        if (!(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)) {
            std::cout << "Error: Doctor not found\n";
            PQclear(res);
            continue;
        }
        PQclear(res);
        break;
    }
    int hosp_id;
    while (true) {
        std::string hospital_id_str = get_validated_input("Enter Hospital ID", true);
        try {
            hosp_id = static_cast<int>(std::stoi(hospital_id_str));
        } catch (...) {
            std::cout << "Error: Enter a number\n";
            continue;
        }
        std::string check_sql = "SELECT 1 FROM hospitals WHERE hospital_id = " + std::to_string(hosp_id)
                                + " AND administrator_id = " + std::to_string(junior_admin_id);
        PGresult *res = PQexec(db.get_connection(), check_sql.c_str());
        if (!(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)) {
            std::cout << "Error: Hospital ID does not match yours\n";
            PQclear(res);
            continue;
        }
        PQclear(res);
        break;
    }
    std::stringstream ss_check;
    ss_check << "SELECT 1 FROM doctors WHERE doctor_id = " << doctor_id
             << " AND " << hosp_id << " = ANY(hospital_ids)";
    PGresult *res_check = PQexec(db.get_connection(), ss_check.str().c_str());
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0)) {
        std::cout << "Error: Hospital ID not found in doctor's list\n";
        PQclear(res_check);
        return;
    }
    PQclear(res_check);
    std::string answer = get_validated_input("Are you sure? (Yes/No)", true);
    if (answer != "Yes") {
        std::cout << "Deletion canceled\n";
        return;
    }
    std::stringstream ss;
    ss << "UPDATE doctors SET hospital_ids = array_remove(hospital_ids, " << hosp_id
       << ") WHERE doctor_id = " << doctor_id;
    PGresult *res = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        std::cout << "Hospital ID removed from doctor's list\n";
    } else {
        std::cout << "Error removing Hospital ID\n";
    }
    PQclear(res);
}