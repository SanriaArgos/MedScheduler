#include "../../include/handlers/patient_schedule.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern database_handler* global_db;

json view_doctor_schedule_for_patient(const json &data) {
    json response;
    // Проверка наличия необходимых полей
    if (!data.contains("region") || !data.contains("settlement_type") ||
        !data.contains("settlement_name") || !data.contains("specialty") ||
        !data.contains("hospital_id") || !data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing required fields (region, settlement_type, settlement_name, specialty, hospital_id, doctor_id)";
        return response;
    }
    // Извлечение параметров
    std::string region = data["region"];
    std::string settlement_type = data["settlement_type"];
    std::string settlement_name = data["settlement_name"];
    std::string specialty = data["specialty"];
    int hospital_id = data["hospital_id"];
    int doctor_id = data["doctor_id"];
    
    std::string doctor_id_str = std::to_string(doctor_id);
    std::string hospital_id_str = std::to_string(hospital_id);
    const char* params[2] = { doctor_id_str.c_str(), hospital_id_str.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "SELECT appointment_date, appointment_time, cabinet_number, patient_id "
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
        record["appointment_date"] = PQgetvalue(res, i, 0);
        record["appointment_time"] = PQgetvalue(res, i, 1);
        record["cabinet_number"] = PQgetvalue(res, i, 2);
        record["patient_id"] = PQgetvalue(res, i, 3);
        schedule.push_back(record);
    }
    PQclear(res);
    response["success"] = true;
    response["schedule"] = schedule;
    return response;
}