#include "junior_admin_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <string>

extern database_handler* global_db;

static bool get_junior_admin_hospital_id(int junior_admin_id, int &out_hospital_id) {
    std::string admin_id_str = std::to_string(junior_admin_id);
    const char* params[1] = { admin_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT hospital_id FROM hospitals WHERE administrator_id = $1 LIMIT 1",
        1, nullptr, params, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        out_hospital_id = std::stoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        return true;
    }
    PQclear(res);
    return false;
}

json junior_admin_schedule(const json &data) {
    json response;
    if (!data.contains("junior_admin_id") || !data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing junior_admin_id or doctor_id";
        return response;
    }
    int junior_admin_id = data["junior_admin_id"];
    int doctor_id = data["doctor_id"];
    
    int my_hospital_id;
    if (!get_junior_admin_hospital_id(junior_admin_id, my_hospital_id)) {
        response["success"] = false;
        response["error"] = "Your hospital not found";
        return response;
    }
    
    std::string doctor_id_str = std::to_string(doctor_id);
    std::string hospital_id_str = std::to_string(my_hospital_id);
    const char* params[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT record_id, appointment_date, appointment_time, cabinet_number, patient_id "
        "FROM records "
        "WHERE doctor_id = $1 AND hospital_id = $2 "
        "AND appointment_date BETWEEN CURRENT_DATE AND (CURRENT_DATE + INTERVAL '7 day') "
        "ORDER BY appointment_date, appointment_time",
        2, nullptr, params, nullptr, nullptr, 0);
    
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] = "Schedule not available";
        if (res) PQclear(res);
        return response;
    }
    
    int rows = PQntuples(res);
    json schedule = json::array();
    for (int i = 0; i < rows; ++i) {
        json record;
        record["record_id"] = PQgetvalue(res, i, 0);
        record["appointment_date"] = PQgetvalue(res, i, 1);
        record["appointment_time"] = PQgetvalue(res, i, 2);
        record["cabinet_number"] = PQgetvalue(res, i, 3);
        record["patient_id"] = PQgetvalue(res, i, 4);
        schedule.push_back(record);
    }
    PQclear(res);
    response["success"] = true;
    response["schedule"] = schedule;
    return response;
}
