#include "junior_admin_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <algorithm>

// Функция для получения hospital_id младшего администратора
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
        // Проверка, что врач существует
        std::stringstream ss_query;
        ss_query << "SELECT 1 FROM doctors WHERE doctor_id = " << doctor_id;
        PGresult *res_doc = PQexec(db.get_connection(), ss_query.str().c_str());
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
    
    // Проверка, что врач ассоциирован с больницей младшего администратора.
    {
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
    }
    
    // Используем параметризованный запрос для выборки расписания.
    const char* paramValues[2];
    std::string doctorIdStr = std::to_string(doctor_id);
    std::string hospitalIdStr = std::to_string(my_hospital_id);
    paramValues[0] = doctorIdStr.c_str();
    paramValues[1] = hospitalIdStr.c_str();
    
    const char* query =
        "SELECT record_id, appointment_date, appointment_time, cabinet_number, "
        "COALESCE(u.last_name, ''), COALESCE(u.first_name, ''), COALESCE(u.patronymic, ''), COALESCE(u.phone, '') "
        "FROM records r "
        "LEFT JOIN users u ON r.patient_id = u.id "
        "WHERE r.doctor_id = $1 AND r.hospital_id = $2 "
        "AND r.appointment_date BETWEEN CURRENT_DATE::date AND (CURRENT_DATE + INTERVAL '14 day')::date "
        "ORDER BY r.appointment_date, r.appointment_time";
    
    PGresult *res = PQexecParams(db.get_connection(), query, 2, NULL, paramValues, NULL, NULL, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Schedule not available. " 
                  << PQerrorMessage(db.get_connection()) << "\n";
        if (res) PQclear(res);
        return;
    }
    
    int rows = PQntuples(res);
    if (rows == 0) {
        std::cout << "No appointments for the next 14 days\n";
    } else {
        std::string current_date = "";
        for (int i = 0; i < rows; ++i) {
            std::string record_id = PQgetvalue(res, i, 0);
            std::string date = PQgetvalue(res, i, 1);
            std::string time = PQgetvalue(res, i, 2);
            std::string cabinet = PQgetvalue(res, i, 3);
            std::string last_name = PQgetvalue(res, i, 4);
            std::string first_name = PQgetvalue(res, i, 5);
            std::string patronymic = PQgetvalue(res, i, 6);
            std::string patient_phone = PQgetvalue(res, i, 7);
            if (date != current_date) {
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }
            std::cout << time << ": (" << record_id << ") cabinet " << cabinet;
            if (patient_phone.empty() || patient_phone == "0" || (last_name.empty() && first_name.empty() && patronymic.empty())) {
                std::cout << ", free";
            } else {
                std::cout << ", " << last_name << " " << first_name << " " << patronymic << ", " << patient_phone;
            }
            std::cout << "\n";
        }
    }
    PQclear(res);
}
