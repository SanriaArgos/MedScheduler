// Handler: post_doctor_rating
// Purpose: Создаёт новый отзыв в таблице rating.
// Input JSON:
//   { "doctor_ref_id":<int>, "user_id":<int>, "text":<string>, "rate":<int>,
//   "address":<string> }
// Output JSON (201 Created):
//   { "success": true, "id": <new_rating_id> }
// Errors:
//   400 Bad Request  – Missing required field
//   404 Not Found    – User not found
//   500 Internal     – при ошибке вставки в БД

#include "../../include/handlers/post_doctor_feedback.hpp"
#include <libpq-fe.h>
#include <ctime>

namespace http = boost::beast::http;
using json = nlohmann::json;

void post_doctor_rating(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверяем обязательные поля (name, date и doctor_ref_id заполняются
    // автоматически)
    const std::vector<std::string> required = {
        "doctor_ref_id", "user_id", "text", "rate", "address"};
    for (const auto &field : required) {
        if (!data.contains(field)) {
            response["success"] = false;
            response["error"] = "Missing field: " + field;
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    // 2. Подготавливаем базовые значения из JSON
    int doctor_ref_id = data["doctor_ref_id"].get<int>();
    int user_id = data["user_id"].get<int>();
    std::string text = data["text"];
    std::string rate_str = std::to_string(data["rate"].get<int>());
    std::string address = data["address"];

    // 3. Получаем ФИО пациента из таблицы users по user_id
    std::string user_id_str = std::to_string(user_id);
    const char *uid_param[1] = {user_id_str.c_str()};
    PGresult *ures = PQexecParams(
        db_handler.get_connection(),
        "SELECT last_name, first_name, patronymic FROM users WHERE id = $1", 1,
        nullptr, uid_param, nullptr, nullptr, 0
    );
    if (!ures || PQresultStatus(ures) != PGRES_TUPLES_OK ||
        PQntuples(ures) == 0) {
        if (ures) {
            PQclear(ures);
        }
        response["success"] = false;
        response["error"] = "User not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    std::string last = PQgetvalue(ures, 0, 0);
    std::string first = PQgetvalue(ures, 0, 1);
    std::string patron = PQgetvalue(ures, 0, 2);
    PQclear(ures);

    // Формируем полное имя
    std::string name = last + " " + first;
    if (!patron.empty()) {
        name += " " + patron;
    }

    // 4. Получаем текущую дату и время в формате "YYYY-MM-DD HH:MM:SS"
    std::time_t now = std::time(nullptr);
    char date_buf[20];
    std::strftime(
        date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now)
    );
    std::string date = date_buf;

    // 5. Проверка на полный дубликат отзыва
    const char *check_params[7] = {
        std::to_string(doctor_ref_id).c_str(),
        std::to_string(user_id).c_str(),
        text.c_str(),
        name.c_str(),
        date.c_str(),
        rate_str.c_str(),
        address.c_str()};
    PGresult *check_res = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM rating WHERE "
        "doctor_ref_id = $1 AND user_id = $2 AND text = $3 "
        "AND name = $4 AND date = $5 AND rate = $6 AND address = $7",
        7, nullptr, check_params, nullptr, nullptr, 0
    );
    if (!check_res || PQresultStatus(check_res) != PGRES_TUPLES_OK) {
        if (check_res) {
            PQclear(check_res);
        }
        response["success"] = false;
        response["error"] = "Database error checking for duplicates";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    if (PQntuples(check_res) > 0) {
        PQclear(check_res);
        response["success"] = false;
        response["error"] = "Identical review already exists";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(check_res);

    // 6. Вставка нового отзыва
    const char *insert_params[7] = {
        std::to_string(doctor_ref_id).c_str(),
        std::to_string(user_id).c_str(),
        text.c_str(),
        name.c_str(),
        date.c_str(),
        rate_str.c_str(),
        address.c_str()};
    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO rating "
        "(doctor_ref_id, user_id, text, name, date, rate, address) "
        "VALUES ($1,$2,$3,$4,$5,$6,$7) RETURNING id",
        7, nullptr, insert_params, nullptr, nullptr, 0
    );
    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) {
            PQclear(pgres);
        }
        response["success"] = false;
        response["error"] = "Database error inserting rating";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int new_id = std::stoi(PQgetvalue(pgres, 0, 0));
    PQclear(pgres);

    // 7. Формируем ответ
    response["success"] = true;
    response["id"] = new_id;
    res.result(http::status::created);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
