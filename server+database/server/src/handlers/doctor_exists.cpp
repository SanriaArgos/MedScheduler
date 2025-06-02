#include "../../include/handlers/doctor_exists.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void doctor_exists(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json result;
    PGconn *conn = db_handler.get_connection();

    // Проверка соединения
    if (PQstatus(conn) != CONNECTION_OK) {
        result = {{"success", false}, {"error", "Database connection failed"}};
        res.result(http::status::service_unavailable);
    }

    // Выполнение запроса
    std::string doctor_id_str = std::to_string(doctor_id);
    const char *paramValues[1] = {doctor_id_str.c_str()};
    PGresult *res_query = PQexecParams(
        conn, "SELECT doctor_id FROM doctors WHERE doctor_id = $1", 1, nullptr,
        paramValues, nullptr, nullptr, 0
    );

    // Обработка ошибки запроса
    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        result = {{"success", false}, {"error", PQerrorMessage(conn)}};
        res.result(http::status::internal_server_error);
    }

    // Формирование ответа
    int rows = PQntuples(res_query);
    if (rows > 0) {
        result = {
            {"success", true},
            {"exists", true},
            {"doctor_id", PQgetvalue(res_query, 0, 0)}};
        res.result(http::status::ok);
    } else {
        result = {{"success", true}, {"exists", false}};
        res.result(http::status::not_found);
    }

    if (res_query) {
        PQclear(res_query);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = result.dump();
}