// Handler: get_doctor_rating_calculated
// Purpose: Вычисляет и возвращает средний рейтинг (AVG(rate)) для указанного
// врача. Input JSON:
//   { "doctor_id": <int> }
// Output JSON (200 OK):
//   { "success": true, "average": <double> }
// Errors:
//   400 Bad Request  – если нет doctor_id
//   500 Internal     – при ошибке базы

#include "../../include/handlers/get_doctor_rating_calculated.hpp"
#include <libpq-fe.h>

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_doctor_rating_calculated(
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

    // 2. Запрос среднего рейтинга из БД
    int doctor_id = data["doctor_id"];
    std::string doctor_id_str = std::to_string(doctor_id);
    const char *params[1] = {doctor_id_str.c_str()};

    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "SELECT AVG(rate) FROM rating WHERE doctor_ref_id = $1", 1, nullptr,
        params, nullptr, nullptr, 0
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

    // 3. Читаем результат и формируем ответ
    char *avg_str = PQgetvalue(pgres, 0, 0);
    double average = avg_str ? atof(avg_str) : 0.0;
    PQclear(pgres);

    response["success"] = true;
    response["average"] = average;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}