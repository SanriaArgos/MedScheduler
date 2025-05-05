// Handler: post_doctor_rating
// Purpose: Создаёт новый отзыв в таблице rating.
// Input JSON:
//   { "doctor_ref_id":<int>,"text":<string>,"name":<string>,
//     "date":<string>,"rate":<int>,"address":<string> }
// Output JSON (201 Created):
//   { "success": true, "id": <new_rating_id> }
// Errors:
//   400 Bad Request  – если отсутствует любое поле
//   500 Internal     – при ошибке вставки в БД

#include "../../include/handlers/post_doctor_feedback.hpp"
#include <libpq-fe.h>

namespace http = boost::beast::http;
using json = nlohmann::json;

void post_doctor_rating(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    const std::vector<std::string> required = {
        "doctor_ref_id", "text", "name", "date", "rate", "address"};
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

    std::string doc_str = std::to_string(data["doctor_ref_id"].get<int>());
    std::string text = data["text"];
    std::string name = data["name"];
    std::string date = data["date"];
    std::string rate_str = std::to_string(data["rate"].get<int>());
    std::string address = data["address"];

    const char *params[6] = {doc_str.c_str(),  text.c_str(),
                             name.c_str(),     date.c_str(),
                             rate_str.c_str(), address.c_str()};
    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO rating (doctor_ref_id, text, name, date, rate, address) "
        "VALUES ($1,$2,$3,$4,$5,$6) RETURNING id",
        6, nullptr, params, nullptr, nullptr, 0
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

    response["success"] = true;
    response["id"] = new_id;
    res.result(http::status::created);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}