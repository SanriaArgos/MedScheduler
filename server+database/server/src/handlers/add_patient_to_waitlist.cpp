// server/src/handlers/add_patient_to_waitlist.cpp

#include "../../include/handlers/add_patient_to_waitlist.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void add_patient_to_waitlist(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверяем только doctor_id и patient_id
    if (!data.contains("doctor_id") || !data.contains("patient_id")) {
        response["success"] = false;
        response["error"]   = "Missing field 'doctor_id' or 'patient_id'";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int doctor_id  = data["doctor_id"].get<int>();
    int patient_id = data["patient_id"].get<int>();
    std::string doctor_id_str  = std::to_string(doctor_id);
    std::string patient_id_str = std::to_string(patient_id);

    // 2. Получаем ФИО и телефон пациента из таблицы users
    const char *user_param[1] = { patient_id_str.c_str() };
    PGresult *ures = PQexecParams(
        db_handler.get_connection(),
        "SELECT last_name, first_name, patronymic, phone "
        "  FROM users "
        " WHERE id = $1::int",
        1, nullptr, user_param, nullptr, nullptr, 0
    );
    if (!ures || PQresultStatus(ures) != PGRES_TUPLES_OK || PQntuples(ures) == 0) {
        if (ures) PQclear(ures);
        response["success"] = false;
        response["error"]   = "Patient not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    std::string last_name  = PQgetvalue(ures, 0, 0);
    std::string first_name = PQgetvalue(ures, 0, 1);
    std::string patronymic = PQgetvalue(ures, 0, 2);
    std::string phone      = PQgetvalue(ures, 0, 3);
    PQclear(ures);

    // 3. Проверяем, что пациента ещё нет в листе ожидания у этого врача
    const char *check_params[2] = {
        doctor_id_str.c_str(),
        patient_id_str.c_str()
    };
    PGresult *check_result = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM waitlist "
        " WHERE doctor_id = $1::int AND patient_id = $2::int",
        2, nullptr, check_params, nullptr, nullptr, 0
    );
    if (!check_result || PQresultStatus(check_result) != PGRES_TUPLES_OK) {
        if (check_result) PQclear(check_result);
        response["success"] = false;
        response["error"]   = "Database error while checking existing entry";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    if (PQntuples(check_result) > 0) {
        PQclear(check_result);
        response["success"] = false;
        response["error"]   = "Patient is already in the waitlist for this doctor";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(check_result);

    // 4. Добавляем запись в waitlist
    const char *insert_params[6] = {
        doctor_id_str.c_str(),
        patient_id_str.c_str(),
        last_name.c_str(),
        first_name.c_str(),
        patronymic.c_str(),
        phone.c_str()
    };
    PGresult *insert_result = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO waitlist "
        "(doctor_id, patient_id, last_name, first_name, patronymic, phone) "
        "VALUES ($1::int, $2::int, $3::text, $4::text, $5::text, $6::text)",
        6, nullptr, insert_params, nullptr, nullptr, 0
    );
    if (!insert_result || PQresultStatus(insert_result) != PGRES_COMMAND_OK) {
        if (insert_result) PQclear(insert_result);
        response["success"] = false;
        response["error"]   = "Failed to add patient to waitlist";
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
