#include "../../include/handlers/get_doctor_feedback_items.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler *global_db;

void get_doctor_rating_items(
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
            "SELECT id,text,name,date,rate,address "
            "FROM rating WHERE doctor_ref_id=$1::int "
            "ORDER BY date DESC",
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
            int n = PQntuples(r);
            json arr = json::array();
            for (int i = 0; i < n; ++i) {
                json it;
                it["id"] = std::stoi(PQgetvalue(r, i, 0));
                it["text"] = PQgetvalue(r, i, 1);
                it["name"] = PQgetvalue(r, i, 2);
                it["date"] = PQgetvalue(r, i, 3);
                it["rate"] = std::stoi(PQgetvalue(r, i, 4));
                it["address"] = PQgetvalue(r, i, 5);
                arr.push_back(std::move(it));
            }
            PQclear(r);
            response["success"] = true;
            response["ratings"] = std::move(arr);
            res.result(http::status::ok);
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
