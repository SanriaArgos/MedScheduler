#include "view_doctor_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <algorithm>

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

void view_doctor_schedule(database_handler &db, int junior_admin_id) {
    std::cout << "\n=== View Doctor Schedule ===\n";
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
    int my_hospital_id = get_junior_admin_hospital_id(db, junior_admin_id);
    if (my_hospital_id == -1) {
        std::cout << "Error: Your hospital not found\n";
        return;
    }
    std::stringstream ss_check;
    ss_check << "SELECT 1 FROM doctors WHERE doctor_id = " << doctor_id
             << " AND " << my_hospital_id << " = ANY(hospital_ids)";
    PGresult *res_check = PQexec(db.get_connection(), ss_check.str().c_str());
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0)) {
        std::cout << "Error: Doctor is not associated with your hospital\n";
        PQclear(res_check);
        return;
    }
    PQclear(res_check);
    std::stringstream ss;
    ss << "SELECT record_id, appointment_date, appointment_time, cabinet_number, patient_id "
       << "FROM records WHERE doctor_id = " << doctor_id
       << " AND hospital_id = " << my_hospital_id
       << " AND appointment_date BETWEEN CURRENT_DATE AND (CURRENT_DATE + INTERVAL '14 day') "
       << "ORDER BY appointment_date, appointment_time";
    PGresult *res = PQexec(db.get_connection(), ss.str().c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Schedule unavailable\n";
        if (res) PQclear(res);
        return;
    }
    int rows = PQntuples(res);
    if (rows == 0) {
        std::cout << "No appointments for the next 14 days\n";
    } else {
        std::string current_date = "";
        for (int i = 0; i < rows; ++i) {
            std::string date = PQgetvalue(res, i, 1);
            if (date != current_date) {
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }
            std::string record_id = PQgetvalue(res, i, 0);
            std::string time = PQgetvalue(res, i, 2);
            std::string cabinet = PQgetvalue(res, i, 3);
            std::string patient_id = PQgetvalue(res, i, 4);
            if (patient_id.empty() || patient_id == "0")
                std::cout << time << ": -\n";
            else
                std::cout << time << ": (" << record_id << ") cabinet_" << cabinet
                          << ", patient_id_" << patient_id << "\n";
        }
    }
    PQclear(res);
}
