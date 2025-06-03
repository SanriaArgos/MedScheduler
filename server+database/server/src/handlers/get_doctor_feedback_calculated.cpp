#include "../../include/handlers/get_doctor_feedback_calculated.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler *global_db;

void get_doctor_rating_calculated(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    if (!data.contains("doctor_id")) {
        response["success"] = false;
        response["error"] = "Missing doctor_id";
        res.result(http::status::bad_request);
    } else {
        int doc_id = data["doctor_id"];
        const char *p = std::to_string(doc_id).c_str();
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT COALESCE(ROUND(AVG(rate)::numeric,1),0) "
            "FROM rating WHERE doctor_ref_id=$1::int",
            1, nullptr, &p, nullptr, nullptr, 0
        );
        if (!r || PQresultStatus(r) != PGRES_TUPLES_OK) {
            if (r) {
                PQclear(r);
            }
            response["success"] = false;
            response["error"] = "DB error";
            res.result(http::status::internal_server_error);
        } else {
            double avg = std::stod(PQgetvalue(r, 0, 0));
            PQclear(r);
            response["success"] = true;
            response["average"] = avg;
            res.result(http::status::ok);
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
