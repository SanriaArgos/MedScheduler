#include "../../include/handlers/display_users.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace http = boost::beast::http;
using json = nlohmann::json;

void display_users_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json result;

    PGconn *conn = db_handler.get_connection();
    std::string query = "SELECT id, last_name, first_name, patronymic, phone, user_type FROM users ORDER BY id";
    PGresult *res_query = PQexec(conn, query.c_str());

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        std::cerr << "Ошибка вывода таблицы пользователей: " << PQerrorMessage(conn) << "\n";
        result["success"] = false;
        result["error"] = "Ошибка при выполнении запроса";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = result.dump();

        if (res_query) PQclear(res_query);
        return;
    }

    int rows = PQntuples(res_query);
    json users = json::array();
    for (int i = 0; i < rows; ++i) {
        json user = {
            {"id", PQgetvalue(res_query, i, 0)},
            {"last_name", PQgetvalue(res_query, i, 1)},
            {"first_name", PQgetvalue(res_query, i, 2)},
            {"patronymic", PQgetvalue(res_query, i, 3)},
            {"phone", PQgetvalue(res_query, i, 4)},
            {"user_type", PQgetvalue(res_query, i, 5)}
        };
        users.push_back(user);
    }

    PQclear(res_query);

    result["success"] = true;
    result["users"] = users;

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = result.dump();
}