#include "../../include/handlers/get_doctor_average_ratings.hpp"
#include <libpq-fe.h>

void get_doctor_average_ratings(
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    PGresult *pgres = PQexec(
        db_handler.get_connection(),
        "SELECT DISTINCT ROUND(AVG(rate)::numeric,1) AS avg_rate "
        "FROM rating "
        "GROUP BY doctor_ref_id "
        "ORDER BY avg_rate DESC"
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
    for (int i = 0; i < PQntuples(pgres); ++i) {
        arr.push_back(std::stod(PQgetvalue(pgres, i, 0)));
    }
    PQclear(pgres);

    response["success"] = true;
    response["doctor_average_ratings"] = arr;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
