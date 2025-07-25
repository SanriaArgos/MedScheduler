// server/src/handlers/get_settlement_types.cpp
#include "../../include/handlers/get_settlement_types.hpp"
#include <libpq-fe.h>

void get_settlement_types(
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    PGresult *pgres = PQexec(
        db_handler.get_connection(),
        "SELECT DISTINCT settlement_type FROM hospitals ORDER BY "
        "settlement_type"
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
    json arr = json::array();
    arr.push_back("-");
    for (int i = 0; i < PQntuples(pgres); ++i) {
        arr.push_back(PQgetvalue(pgres, i, 0));
    }
    PQclear(pgres);

    response["success"] = true;
    response["settlement_types"] = arr;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
