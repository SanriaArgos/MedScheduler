#include "../../include/handlers/get_doctor_average_ratings.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler *global_db;

void get_doctor_average_ratings(
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    // Формула: вес 2 для отзывов с текстом, вес 1 для «только оценки»
    PGresult *r = PQexec(
        db_handler.get_connection(),
        R"SQL(
          SELECT ROUND(
            (SUM(rate * CASE WHEN text IS NOT NULL AND length(text)>0 THEN 2 ELSE 1 END)
             / SUM(CASE WHEN text IS NOT NULL AND length(text)>0 THEN 2 ELSE 1 END)
            )::numeric,1
          ) AS avg_rate
          FROM rating
          GROUP BY doctor_ref_id
          ORDER BY avg_rate DESC
        )SQL"
    );
    if (!r || PQresultStatus(r) != PGRES_TUPLES_OK) {
        if (r) {
            PQclear(r);
        }
        response["success"] = false;
        response["error"] = "DB error";
        res.result(http::status::internal_server_error);
    } else {
        int n = PQntuples(r);
        json arr = json::array();
        arr.push_back("-");  // плейсхолдер
        for (int i = 0; i < n; ++i) {
            arr.push_back(std::stod(PQgetvalue(r, i, 0)));
        }
        PQclear(r);
        response["success"] = true;
        response["doctor_average_ratings"] = std::move(arr);
        res.result(http::status::ok);
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
