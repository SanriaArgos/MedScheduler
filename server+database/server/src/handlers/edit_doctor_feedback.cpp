// Handler: edit_doctor_rating
// Purpose: Обновляет существующий отзыв. Доступно только автору отзыва.
// Input JSON:
//   { "rating_id":<int>, "doctor_ref_id":<int>, "text":<string>,
//     "name":<string>, "date":<string>, "rate":<int>,
//     "address":<string>, "user_id":<int> }
// Output JSON (200 OK):
//   { "success": true }
// Errors:
//   400 Bad Request  – отсутствует любое поле
//   404 Not Found    – отзыв не найден или user_id не найден
//   403 Forbidden    – пользователь не является автором отзыва
//   500 Internal     – ошибка БД

#include "../../include/handlers/edit_doctor_feedback.hpp"
#include <libpq-fe.h>

namespace http = boost::beast::http;
using json = nlohmann::json;

void edit_doctor_feedback(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    const std::vector<std::string> required = {
        "rating_id", "doctor_ref_id", "text",    "name",
        "date",      "rate",          "address", "user_id"};
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

    int rating_id = data["rating_id"];
    int user_id = data["user_id"];
    int doctor_ref_id = data["doctor_ref_id"];
    std::string text = data["text"];
    std::string name = data["name"];
    std::string date = data["date"];
    int rate = data["rate"];
    std::string address = data["address"];

    std::string rid_str = std::to_string(rating_id);
    const char *auth_params[1] = {rid_str.c_str()};
    PGresult *auth_res = PQexecParams(
        db_handler.get_connection(), "SELECT user_id FROM rating WHERE id = $1",
        1, nullptr, auth_params, nullptr, nullptr, 0
    );
    if (!auth_res || PQresultStatus(auth_res) != PGRES_TUPLES_OK ||
        PQntuples(auth_res) == 0) {
        if (auth_res) {
            PQclear(auth_res);
        }
        response["success"] = false;
        response["error"] = "Rating not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int author_id = std::stoi(PQgetvalue(auth_res, 0, 0));
    PQclear(auth_res);

    if (user_id != author_id) {
        response["success"] = false;
        response["error"] = "Permission denied: only author can edit";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    const char *upd_params[7] = {
        std::to_string(doctor_ref_id).c_str(),
        text.c_str(),
        name.c_str(),
        date.c_str(),
        std::to_string(rate).c_str(),
        address.c_str(),
        std::to_string(rating_id).c_str()};
    PGresult *upd_res = PQexecParams(
        db_handler.get_connection(),
        "UPDATE rating SET "
        "doctor_ref_id = $1, text = $2, name = $3, date = $4, rate = $5, "
        "address = $6 "
        "WHERE id = $7",
        7, nullptr, upd_params, nullptr, nullptr, 0
    );
    if (!upd_res || PQresultStatus(upd_res) != PGRES_COMMAND_OK) {
        if (upd_res) {
            PQclear(upd_res);
        }
        response["success"] = false;
        response["error"] = "Error updating rating";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(upd_res);

    response["success"] = true;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}