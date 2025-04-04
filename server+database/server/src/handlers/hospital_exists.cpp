#include "../../include/handlers/hospital_exists.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace http = boost::beast::http;
using json = nlohmann::json;

void hospital_exists(int hospital_id, http::response<http::string_body>& res, database_handler& db_handler) {
    json result;
    PGconn* conn = db_handler.get_connection();

    if (PQstatus(conn) != CONNECTION_OK) {
        result = {
            {"success", false},
            {"error", "Database connection failed"}
        };
        res.result(http::status::service_unavailable);
    }

    std::string hospital_id_str = std::to_string(hospital_id);
    const char* paramValues[1] = { hospital_id_str.c_str() };
    PGresult* res_query = PQexecParams(conn,
        "SELECT hospital_id FROM hospitals WHERE hospital_id = $1",
        1, nullptr, paramValues, nullptr, nullptr, 0);

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        result = {
            {"success", false},
            {"error", PQerrorMessage(conn)}
        };
        res.result(http::status::internal_server_error);
    }

    int rows = PQntuples(res_query);
    if (rows > 0) {
        result = {
            {"success", true},
            {"exists", true},
            {"hospital_id", PQgetvalue(res_query, 0, 0)}
        };
        res.result(http::status::ok);
    } else {
        result = {
            {"success", true},
            {"exists", false}
        };
        res.result(http::status::not_found);
    }

    if (res_query) PQclear(res_query);

    res.set(http::field::content_type, "application/json");
    res.body() = result.dump();
}
