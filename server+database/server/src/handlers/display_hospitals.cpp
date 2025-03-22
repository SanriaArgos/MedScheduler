#include "../../include/handlers/display_hospitals.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace http = boost::beast::http;
using json = nlohmann::json;

void display_hospitals_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json result;

    PGconn *conn = db_handler.get_connection();
    std::string query = "SELECT hospital_id, region, settlement_type, settlement_name, street, house, full_name, administrator_id FROM hospitals ORDER BY hospital_id";
    PGresult *res_query = PQexec(conn, query.c_str());

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        std::cerr << "Ошибка вывода таблицы больниц: " << PQerrorMessage(conn) << "\n";
        result["success"] = false;
        result["error"] = "Ошибка при выполнении запроса";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = result.dump();

        if (res_query) PQclear(res_query);
        return;
    }

    int rows = PQntuples(res_query);
    json hospitals = json::array();
    for (int i = 0; i < rows; ++i) {
        json hospital = {
            {"hospital_id", PQgetvalue(res_query, i, 0)},
            {"region", PQgetvalue(res_query, i, 1)},
            {"settlement_type", PQgetvalue(res_query, i, 2)},
            {"settlement_name", PQgetvalue(res_query, i, 3)},
            {"street", PQgetvalue(res_query, i, 4)},
            {"house", PQgetvalue(res_query, i, 5)},
            {"full_name", PQgetvalue(res_query, i, 6)},
            {"administrator_id", PQgetvalue(res_query, i, 7)}
        };
        hospitals.push_back(hospital);
    }

    PQclear(res_query);

    result["success"] = true;
    result["hospitals"] = hospitals;

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = result.dump();
}