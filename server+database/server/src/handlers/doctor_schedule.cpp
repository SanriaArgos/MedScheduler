#include "../../include/handlers/doctor_schedule.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern database_handler* global_db;

json display_doctor_schedule(const json &data) {
    json response;
    if (!data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing doctor_id";
        return response;
    }
    int doctor_id = data["doctor_id"];
    std::string doctor_id_str = std::to_string(doctor_id);
    const char* params[1] = { doctor_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT r.appointment_date, r.appointment_time, h.full_name, r.cabinet_number, "
        "u.last_name, u.first_name, u.patronymic, u.phone "
        "FROM records r "
        "JOIN hospitals h ON r.hospital_id = h.hospital_id "
        "LEFT JOIN users u ON r.patient_id = u.id "
        "WHERE r.doctor_id = $1 "
        "AND r.appointment_date BETWEEN CURRENT_DATE AND (CURRENT_DATE + INTERVAL '7 day') "
        "ORDER BY r.appointment_date, r.appointment_time",
        1, nullptr, params, nullptr, nullptr, 0);
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
        record["appointment_date"] = PQgetvalue(res, i, 0);
        record["appointment_time"] = PQgetvalue(res, i, 1);
        record["hospital_full_name"] = PQgetvalue(res, i, 2);
        record["cabinet_number"] = PQgetvalue(res, i, 3);
        record["patient_last_name"] = PQgetvalue(res, i, 4);
        record["patient_first_name"] = PQgetvalue(res, i, 5);
        record["patient_patronymic"] = PQgetvalue(res, i, 6);
        record["patient_phone"] = PQgetvalue(res, i, 7);
        schedule.push_back(record);
    }
    PQclear(res);
    response["success"] = true;
    response["schedule"] = schedule;
    return response;
}