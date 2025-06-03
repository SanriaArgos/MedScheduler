#include "../../include/handlers/get_doctor_statistics.hpp"
#include "../../include/get_hospital_id.hpp"
#include <libpq-fe.h>
#include <sstream>
#include <iomanip>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void get_doctor_statistics(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверка наличия полей junior_admin_id и doctor_phone
    if (!data.contains("junior_admin_id") || !data.contains("doctor_phone")) {
        response["success"] = false;
        response["error"] = "Missing 'junior_admin_id' or 'doctor_phone'";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int junior_admin_id = data["junior_admin_id"].get<int>();
    std::string doctor_phone = data["doctor_phone"].get<std::string>();

    // 2. Получаем hospital_id младшего администратора
    int hospital_id = get_hospital_id_admin(junior_admin_id);
    if (hospital_id < 0) {
        response["success"] = false;
        response["error"] = "Junior administrator's hospital not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 3. Находим user_id врача по номеру телефона и проверяем user_type
    const char* phone_param[1] = { doctor_phone.c_str() };
    PGresult *r1 = PQexecParams(
        db_handler.get_connection(),
        "SELECT id, user_type FROM users WHERE phone = $1 LIMIT 1",
        1, nullptr, phone_param, nullptr, nullptr, 0
    );
    if (!r1 || PQresultStatus(r1) != PGRES_TUPLES_OK || PQntuples(r1) == 0) {
        if (r1) PQclear(r1);
        response["success"] = false;
        response["error"] = "Doctor not found by phone";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int user_id = std::stoi(PQgetvalue(r1, 0, 0));
    std::string user_type = PQgetvalue(r1, 0, 1);
    PQclear(r1);
    if (user_type != "doctor") {
        response["success"] = false;
        response["error"] = "User is not a doctor";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4. Получаем doctor_id и price, а также проверяем, что hospital_id в hospital_ids
    std::string user_id_str = std::to_string(user_id);
    const char* doc_param[2] = { user_id_str.c_str(), std::to_string(hospital_id).c_str() };
    PGresult *r2 = PQexecParams(
        db_handler.get_connection(),
        "SELECT doctor_id, price FROM doctors "
        " WHERE user_id = $1::int AND $2::int = ANY(hospital_ids) LIMIT 1",
        2, nullptr, doc_param, nullptr, nullptr, 0
    );
    if (!r2 || PQresultStatus(r2) != PGRES_TUPLES_OK || PQntuples(r2) == 0) {
        if (r2) PQclear(r2);
        response["success"] = false;
        response["error"] = "Doctor is not attached to your hospital";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int doctor_id = std::stoi(PQgetvalue(r2, 0, 0));
    double price = std::stod(PQgetvalue(r2, 0, 1));
    PQclear(r2);

    // 5. Считаем количество пациентов в текущем месяце
    // Диапазон: с первого числа текущего месяца до первого числа следующего месяца (не включая)
    const char *stats_params[2] = {
        std::to_string(doctor_id).c_str(),
        std::to_string(hospital_id).c_str()
    };
    PGresult *r3 = PQexecParams(
        db_handler.get_connection(),
        "SELECT COUNT(*) FROM records "
        " WHERE doctor_id = $1::int AND hospital_id = $2::int "
        "   AND appointment_date >= date_trunc('month', CURRENT_DATE) "
        "   AND appointment_date < date_trunc('month', CURRENT_DATE) + INTERVAL '1 month' "
        "   AND patient_id IS NOT NULL",
        2, nullptr, stats_params, nullptr, nullptr, 0
    );
    if (!r3 || PQresultStatus(r3) != PGRES_TUPLES_OK) {
        if (r3) PQclear(r3);
        response["success"] = false;
        response["error"] = "Database error counting patients";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int patients_count = std::stoi(PQgetvalue(r3, 0, 0));
    PQclear(r3);

    // 6. Получаем средний рейтинг врача (округлённый до десятых)
    const char *rate_param[1] = { std::to_string(doctor_id).c_str() };
    PGresult *r4 = PQexecParams(
        db_handler.get_connection(),
        "SELECT ROUND(AVG(rate)::numeric, 1) "
        "  FROM rating WHERE doctor_ref_id = $1::int",
        1, nullptr, rate_param, nullptr, nullptr, 0
    );
    if (!r4 || PQresultStatus(r4) != PGRES_TUPLES_OK) {
        if (r4) PQclear(r4);
        response["success"] = false;
        response["error"] = "Database error calculating rating";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    double average_rating = 0.0;
    if (PQgetisnull(r4, 0, 0) == 0) {
        average_rating = std::stod(PQgetvalue(r4, 0, 0));
    }
    PQclear(r4);

    // 7. Формируем и возвращаем результат
    response["success"] = true;
    response["patients_count"] = patients_count;
    response["average_rating"] = average_rating;
    response["price"] = price;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
