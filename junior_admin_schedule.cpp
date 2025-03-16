#include "junior_admin_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <algorithm>
#include <string>

// Returns the hospital ID associated with the given junior administrator, using a parameterized query.
static int get_junior_admin_hospital_id(database_handler &db, int junior_admin_id) {
    int hospital_id = -1;
    const char* paramValues[1];
    std::string admin_id_str = std::to_string(junior_admin_id);
    paramValues[0] = admin_id_str.c_str();
    const char* query = "SELECT hospital_id FROM hospitals WHERE administrator_id = $1 LIMIT 1";
    PGresult *res = PQexecParams(db.get_connection(), query, 1, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        hospital_id = static_cast<int>(std::stoi(PQgetvalue(res, 0, 0)));
    }
    PQclear(res);
    return hospital_id;
}

void junior_admin_schedule(database_handler &db, int junior_admin_id) {
    std::cout << "\n=== View Doctor Schedule ===\n";
    
    int doctor_id;
    while (true) {
        std::string doctor_id_str = get_validated_input("Enter Doctor ID", true);
        try {
            doctor_id = static_cast<int>(std::stoi(doctor_id_str));
        } catch (...) {
            std::cout << "Error: Enter a valid number\n";
            continue;
        }
        // Check doctor existence using a parameterized query.
        const char* paramValues[1];
        std::string docIdStr = std::to_string(doctor_id);
        paramValues[0] = docIdStr.c_str();
        const char* query_doc = "SELECT 1 FROM doctors WHERE doctor_id = $1";
        PGresult *res_doc = PQexecParams(db.get_connection(), query_doc, 1, NULL, paramValues, NULL, NULL, 0);
        if (!(PQresultStatus(res_doc) == PGRES_TUPLES_OK && PQntuples(res_doc) > 0)) {
            std::cout << "Error: Doctor not found\n";
            PQclear(res_doc);
            continue;
        }
        PQclear(res_doc);
        break;
    }
    
    int my_hospital_id = get_junior_admin_hospital_id(db, junior_admin_id);
    if (my_hospital_id == -1) {
        std::cout << "Error: Your hospital not found\n";
        return;
    }
    
    // Check that the doctor is associated with the junior admin's hospital.
    {
        const char* paramValues2[2];
        std::string docIdStr = std::to_string(doctor_id);
        std::string hospIdStr = std::to_string(my_hospital_id);
        paramValues2[0] = docIdStr.c_str();
        paramValues2[1] = hospIdStr.c_str();
        const char* query_check = "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)";
        PGresult *res_check = PQexecParams(db.get_connection(), query_check, 2, NULL, paramValues2, NULL, NULL, 0);
        if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0)) {
            std::cout << "Error: Doctor is not associated with your hospital\n";
            PQclear(res_check);
            return;
        }
        PQclear(res_check);
    }
    
    // Retrieve schedule using a parameterized query.
    const char* paramValues3[2];
    std::string doctorIdStr = std::to_string(doctor_id);
    std::string hospitalIdStr = std::to_string(my_hospital_id);
    paramValues3[0] = doctorIdStr.c_str();
    paramValues3[1] = hospitalIdStr.c_str();
    const char* query =
        "SELECT record_id, appointment_date, appointment_time, cabinet_number, patient_id "
        "FROM records "
        "WHERE doctor_id = $1 AND hospital_id = $2 "
        "AND appointment_date BETWEEN CURRENT_DATE::date AND (CURRENT_DATE + INTERVAL '14 day')::date "
        "ORDER BY appointment_date, appointment_time";
    
    PGresult *res = PQexecParams(db.get_connection(), query, 2, NULL, paramValues3, NULL, NULL, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Schedule not available. " << PQerrorMessage(db.get_connection()) << "\n";
        if (res) PQclear(res);
        return;
    }
    
    int rows = PQntuples(res);
    if (rows == 0) {
        std::cout << "No appointments for the next 14 days\n";
    } else {
        std::string current_date;
        for (int i = 0; i < rows; ++i) {
            std::string record_id = PQgetvalue(res, i, 0);
            std::string date = PQgetvalue(res, i, 1);
            std::string time = PQgetvalue(res, i, 2);
            std::string cabinet = PQgetvalue(res, i, 3);
            std::string patient_id = PQgetvalue(res, i, 4);
            if (date != current_date) {
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }
            std::cout << time << ": (" << record_id << ") cabinet " << cabinet;
            if (patient_id.empty() || patient_id == "0")
                std::cout << ", free";
            else
                std::cout << ", patient " << patient_id;
            std::cout << "\n";
        }
    }
    PQclear(res);
}
