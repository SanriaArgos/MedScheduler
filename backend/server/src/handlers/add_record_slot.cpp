#include "../../include/handlers/add_record_slot.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler *global_db;

int get_junior_admin_hospital_id(int junior_admin_id) {
    int hospital_id = -1;
    std::string admin_id_str = std::to_string(junior_admin_id);
    const char *params[1] = {admin_id_str.c_str()};
    PGresult *res = PQexecParams(
        global_db->get_connection(),
        "SELECT hospital_id FROM hospitals WHERE administrator_id = $1 LIMIT 1",
        1, NULL, params, NULL, NULL, 0
    );
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        hospital_id = std::stoi(PQgetvalue(res, 0, 0));
    }
    PQclear(res);
    return hospital_id;
}

void add_record_slot(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // Проверка на наличие обязательных полей
    bool has_error = false;

    if (!data.contains("doctor_id")) {
        std::cerr << "Error: Missing field 'doctor_id'\n";
        response["missing_fields"].push_back("doctor_id");
        has_error = true;
    }
    if (!data.contains("date")) {
        std::cerr << "Error: Missing field 'date'\n";
        response["missing_fields"].push_back("date");
        has_error = true;
    }
    if (!data.contains("time")) {
        std::cerr << "Error: Missing field 'time'\n";
        response["missing_fields"].push_back("time");
        has_error = true;
    }
    if (!data.contains("hospital_id")) {
        std::cerr << "Error: Missing field 'hospital_id'\n";
        response["missing_fields"].push_back("hospital_id");
        has_error = true;
    }
    if (!data.contains("cabinet")) {
        std::cerr << "Error: Missing field 'cabinet'\n";
        response["missing_fields"].push_back("cabinet");
        has_error = true;
    }
    if (!data.contains("junior_admin_id")) {
        std::cerr << "Error: Missing field 'junior_admin_id'\n";
        response["missing_fields"].push_back("junior_admin_id");
        has_error = true;
    }

    if (has_error) {
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int doctor_id = data["doctor_id"];
    std::string date = data["date"];
    std::string time_val = data["time"];
    int hospital_id = data["hospital_id"];
    int cabinet = data["cabinet"];
    int junior_admin_id = data["junior_admin_id"];

    // Проверяем принадлежность hospital_id младшему администратору.
    std::string hosp_id_str = std::to_string(hospital_id);
    std::string junior_admin_id_str = std::to_string(junior_admin_id);
    const char *params_check[2] = {
        hosp_id_str.c_str(), junior_admin_id_str.c_str()};
    PGresult *res_check = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE hospital_id = $1 AND administrator_id = "
        "$2",
        2, NULL, params_check, NULL, NULL, 0
    );
    if (!(PQresultStatus(res_check) == PGRES_TUPLES_OK &&
          PQntuples(res_check) > 0)) {
        std::cerr << "Error: Hospital ID does not match yours\n";
        PQclear(res_check);
        response["success"] = false;
        response["error"] = "Hospital ID does not match yours";

        res.result(http::status::forbidden);  // 403 Forbidden
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(res_check);

    int my_hospital_id = get_junior_admin_hospital_id(junior_admin_id);
    if (my_hospital_id == -1) {
        std::cerr << "Error: Your hospital not found\n";
        response["success"] = false;
        response["error"] = "Your hospital not found";

        res.result(http::status::not_found);  // 404 Not Found
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string doctor_id_str = std::to_string(doctor_id);
    std::string my_hosp_id_str = std::to_string(my_hospital_id);
    const char *params_check_doc[2] = {
        doctor_id_str.c_str(), my_hosp_id_str.c_str()};
    PGresult *res_check_doc = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids)",
        2, NULL, params_check_doc, NULL, NULL, 0
    );
    if (!(PQresultStatus(res_check_doc) == PGRES_TUPLES_OK &&
          PQntuples(res_check_doc) > 0)) {
        std::cerr << "Error: Doctor is not associated with your hospital\n";
        PQclear(res_check_doc);
        response["success"] = false;
        response["error"] = "Doctor is not associated with your hospital";

        res.result(http::status::forbidden);  // 403 Forbidden
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(res_check_doc);

    const char *params_check_slot[3] = {
        doctor_id_str.c_str(), date.c_str(), time_val.c_str()};
    PGresult *res_check_slot = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM records WHERE doctor_id = $1 AND appointment_date=$2 "
        "AND appointment_time=$3",
        3, NULL, params_check_slot, NULL, NULL, 0
    );
    if (PQresultStatus(res_check_slot) == PGRES_TUPLES_OK &&
        PQntuples(res_check_slot) > 0) {
        std::cerr << "Error: The slot for this date and time already exists\n";
        PQclear(res_check_slot);
        response["success"] = false;
        response["error"] = "The slot for this date and time already exists";

        res.result(http::status::forbidden);  // 403 Forbidden
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(res_check_slot);

    std::string cabinet_str = std::to_string(cabinet);
    const char *params_ins[5] = {
        doctor_id_str.c_str(), date.c_str(), time_val.c_str(),
        hosp_id_str.c_str(), cabinet_str.c_str()};
    PGresult *res_ins = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO records (doctor_id, appointment_date, appointment_time, "
        "hospital_id, cabinet_number, patient_id) "
        "VALUES ($1, $2, $3, $4, $5, NULL)",
        5, NULL, params_ins, NULL, NULL, 0
    );
    if (PQresultStatus(res_ins) == PGRES_COMMAND_OK) {
        std::cerr << "Appointment slot added\n";
        response["success"] = true;
        response["message"] = "Appointment slot added";

        res.result(http::status::ok);  // 200 OK
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
    } else {
        std::cerr << "Error adding appointment slot\n";
        response["success"] = false;
        response["error"] = "Error adding appointment slot";

        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
    }
    PQclear(res_ins);
}