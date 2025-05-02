// Handler: get_doctor_rating_items
// Purpose: Возвращает список всех отзывов (ratings) для указанного врача.
// Input JSON:
//   { "doctor_id": <int> }
// Output JSON (200 OK):
//   { "success": true, "ratings": [ {id,text,name,date,rate,address}, ... ] }
// Errors:
//   400 Bad Request  – если нет doctor_id
//   500 Internal     – при ошибке базы

#include "../../include/handlers/get_doctor_rating_items.hpp"
#include <libpq-fe.h>

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_doctor_rating_items(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверяем обязательный параметр
    if (!data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing doctor_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 2. Запрашиваем все поля отзывов из БД
    int doctor_id = data["doctor_id"];
    std::string id_str = std::to_string(doctor_id);
    const char *params[1] = {id_str.c_str()};

    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "SELECT id, text, name, date, rate, address "
        "FROM rating WHERE doctor_ref_id = $1 ORDER BY date DESC",
        1, nullptr, params, nullptr, nullptr, 0
    );
    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) {
            PQclear(pgres);
        }
        response["success"] = false;
        response["error"] = "Database error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 3. Собираем массив отзывов
    int rows = PQntuples(pgres);
    json items = json::array();
    for (int i = 0; i < rows; ++i) {
        json item;
        item["id"] = std::stoi(PQgetvalue(pgres, i, 0));
        item["text"] = PQgetvalue(pgres, i, 1);
        item["name"] = PQgetvalue(pgres, i, 2);
        item["date"] = PQgetvalue(pgres, i, 3);
        item["rate"] = std::stoi(PQgetvalue(pgres, i, 4));
        item["address"] = PQgetvalue(pgres, i, 5);
        items.push_back(item);
    }
    PQclear(pgres);

    // 4. Отправляем результат клиенту
    response["success"] = true;
    response["ratings"] = items;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
