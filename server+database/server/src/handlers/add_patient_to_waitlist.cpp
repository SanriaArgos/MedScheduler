#include "../../include/handlers/add_patient_to_waitlist.hpp"
#include <libpq-fe.h>
#include <iostream>

void add_patient_to_waitlist(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    
    // Проверка обязательных полей
    for (auto &f :
         {"doctor_id", "patient_id", "last_name", "first_name", "patronymic",
          "phone"}) {
        if (!data.contains(f)) {
            response["success"] = false;
            response["error"] = std::string("Missing field '") + f + "'";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    std::string doctor_id_str = std::to_string(data["doctor_id"].get<int>());
    std::string patient_id_str = std::to_string(data["patient_id"].get<int>());
    std::string last_name_user_str = data["last_name"].get<std::string>();
    std::string first_name_user_str = data["first_name"].get<std::string>();
    std::string middle_name_user_str = data["patronymic"].get<std::string>();
    std::string phone_user_str = data["phone"].get<std::string>();

    // Проверяем, не находится ли пациент уже в листе ожидания у этого врача
    const char *check_params[2] = {doctor_id_str.c_str(), patient_id_str.c_str()};
    PGresult *check_result = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM waitlist WHERE doctor_id = $1 AND patient_id = $2",
        2, nullptr, check_params, nullptr, nullptr, 0
    );

    if (!check_result || PQresultStatus(check_result) != PGRES_TUPLES_OK) {
        if (check_result) PQclear(check_result);
        response["success"] = false;
        response["error"] = "Database error while checking existing entry";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Если запись уже существует, возвращаем ошибку
    if (PQntuples(check_result) > 0) {
        PQclear(check_result);
        response["success"] = false;
        response["error"] = "Patient is already in the waitlist for this doctor";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(check_result);

    // Если пациента нет в листе ожидания, добавляем
    const char *insert_params[6] = {
        doctor_id_str.c_str(),        patient_id_str.c_str(),
        last_name_user_str.c_str(),   first_name_user_str.c_str(),
        middle_name_user_str.c_str(), phone_user_str.c_str()};
    
    PGresult *insert_result = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO waitlist "
        "(doctor_id, patient_id, last_name, first_name, patronymic, phone) "
        "VALUES ($1, $2, $3, $4, $5, $6)",
        6, nullptr, insert_params, nullptr, nullptr, 0
    );

    if (!insert_result || PQresultStatus(insert_result) != PGRES_COMMAND_OK) {
        if (insert_result) PQclear(insert_result);
        response["success"] = false;
        response["error"] = "Failed to add patient to waitlist";
        res.result(http::status::internal_server_error);
    } else {
        PQclear(insert_result);
        response["success"] = true;
        response["message"] = "Patient added to waitlist successfully";
        res.result(http::status::ok);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}