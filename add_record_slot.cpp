#include "add_record_slot.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <algorithm>

extern database_handler* global_db;

// Вспомогательная функция для получения hospital_id младшего администратора.
static int get_junior_admin_hospital_id(int junior_admin_id) {
    int hospital_id = -1;
    std::string admin_id_str = std::to_string(junior_admin_id);
    const char* params[1] = { admin_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT hospital_id FROM hospitals WHERE administrator_id = $1 LIMIT 1",
        1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        hospital_id = static_cast<int>(std::stoi(PQgetvalue(res, 0, 0)));
    }
    PQclear(res);
    return hospital_id;
}

bool add_record_slot(int doctor_id,
                     const std::string &date,
                     const std::string &time,
                     int hospital_id,
                     int cabinet,
                     int junior_admin_id) {
    // Проверка: hospital_id принадлежит младшему администратору.
    std::string admin_id_str = std::to_string(junior_admin_id);
    std::string hosp_id_str = std::to_string(hospital_id);
    const char* params_check[2] = { hosp_id_str.c_str(), admin_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = $2",
        2, NULL, params_check, NULL, NULL, 0);
    if (!(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)) {
        std::cerr << "Error: Hospital ID does not match yours\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    
    int my_hospital_id = get_junior_admin_hospital_id(junior_admin_id);
    if (my_hospital_id == -1) {
        std::cerr << "Error: Your hospital not found\n";
        return false;
    }
    std::string my_hosp_id_str = std::to_string(my_hospital_id);
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params_check_doc[2] = { doctor_id_str.c_str(), my_hosp_id_str.c_str() };
    PGresult *res_check = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check_doc, NULL, NULL, 0);
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0)) {
        std::cerr << "Error: Doctor is not associated with your hospital\n";
        PQclear(res_check);
        return false;
    }
    PQclear(res_check);
    
    // Вставка записи с использованием параметризованного запроса.
    std::string doctor_id_str2 = std::to_string(doctor_id);
    std::string cabinet_str = std::to_string(cabinet);
    const char* params_ins[5] = { doctor_id_str2.c_str(), date.c_str(), time.c_str(), hosp_id_str.c_str(), cabinet_str.c_str() };
    PGresult *res_ins = PQexecParams(global_db->get_connection(),
        "INSERT INTO records (doctor_id, appointment_date, appointment_time, hospital_id, cabinet_number, patient_id) "
        "VALUES ($1, $2, $3, $4, $5, NULL)",
        5, NULL, params_ins, NULL, NULL, 0);
    if (PQresultStatus(res_ins) == PGRES_COMMAND_OK) {
        std::cerr << "Appointment slot added\n";
        PQclear(res_ins);
        return true;
    } else {
        std::cerr << "Error adding appointment slot\n";
        PQclear(res_ins);
        return false;
    }
}
