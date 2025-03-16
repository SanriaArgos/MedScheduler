#include "doctor_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <string>

void display_doctor_schedule(database_handler &db, int doctor_id) {
    std::cout << "\n=== View Schedule ===\n";
    
    std::stringstream ss;
    ss << "SELECT r.appointment_date, r.appointment_time, h.full_name, r.cabinet_number, "
          "u.last_name, u.first_name, u.patronymic, u.phone "
          "FROM records r "
          "JOIN hospitals h ON r.hospital_id = h.hospital_id "
          "LEFT JOIN users u ON r.patient_id = u.id "
          "WHERE r.doctor_id = " << doctor_id 
       << " AND r.appointment_date BETWEEN CURRENT_DATE::date AND (CURRENT_DATE + INTERVAL '14 day')::date "
          "ORDER BY r.appointment_date, r.appointment_time";
    
    PGresult *res = PQexec(db.get_connection(), ss.str().c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Schedule not available\n";
        if (res) PQclear(res);
        return;
    }
    
    int rows = PQntuples(res);
    if (rows == 0) {
        std::cout << "No appointments for the next 14 days\n";
    } else {
        std::string current_date = "";
        for (int i = 0; i < rows; ++i) {
            std::string date = PQgetvalue(res, i, 0);
            std::string time = PQgetvalue(res, i, 1);
            std::string hospital = PQgetvalue(res, i, 2);
            std::string cabinet = PQgetvalue(res, i, 3);
            std::string last_name = PQgetvalue(res, i, 4);
            std::string first_name = PQgetvalue(res, i, 5);
            std::string patronymic = PQgetvalue(res, i, 6);
            std::string patient_phone = PQgetvalue(res, i, 7);
            if (date != current_date) {
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }
            std::cout << time << ": " << hospital << ", cabinet " << cabinet;
            if (!last_name.empty() || !first_name.empty() || !patronymic.empty()) {
                std::cout << ", " << last_name << " " << first_name << " " << patronymic 
                          << ", " << patient_phone;
            }
            std::cout << "\n";
        }
    }
    PQclear(res);
}
