#include "add_record_slot.hpp"
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

void add_record_slot(database_handler &db, int junior_admin_id) {
    std::cout << "\n=== Add Appointment Slot ===\n";
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
    std::string date = get_validated_input("Enter Date (YYYY-MM-DD)", true);
    std::regex date_regex(R"(^20[2-9]\d-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])$)");
    while (!std::regex_match(date, date_regex)) {
        std::cout << "Error: Date must be in format YYYY-MM-DD, year >= 2025\n";
        date = get_validated_input("Enter Date (YYYY-MM-DD)", true);
    }
    std::string time = get_validated_input("Enter Time (HH:MM)", true);
    std::regex time_regex(R"(^([01]\d|2[0-3]):([0-5]\d)$)");
    while (!std::regex_match(time, time_regex)) {
        std::cout << "Error: Time must be in format HH:MM\n";
        time = get_validated_input("Enter Time (HH:MM)", true);
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
    int office;
    while (true) {
        std::string office_str = get_validated_input("Enter Office Number (>=1)", true);
        try {
            office = static_cast<int>(std::stoi(office_str));
            if (office < 1) {
                std::cout << "Error: Office number must be >= 1\n";
                continue;
            }
        } catch (...) {
            std::cout << "Error: Enter a number\n";
            continue;
        }
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
    ss << "INSERT INTO records (doctor_id, appointment_date, appointment_time, hospital_id, office_number, patient_id) VALUES ("
       << doctor_id << ", '" << date << "', '" << time << "', " << hosp_id << ", " << office << ", NULL)";
    PGresult *res = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        std::cout << "Appointment slot added\n";
    } else {
        std::cout << "Error adding appointment slot\n";
    }
    PQclear(res);
}
