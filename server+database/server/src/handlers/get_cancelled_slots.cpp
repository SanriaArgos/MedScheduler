#include "../../include/handlers/get_cancelled_slots.hpp"
#include <libpq-fe.h>

void get_cancelled_slots(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    // URL: /get_cancelled_slots?doctor_id=<int>
    auto url = req.target().to_string();
    auto pos = url.find("doctor_id=");
    json response;
    if (pos == std::string::npos) {
        response["success"] = false;
        response["error"] = "Missing doctor_id";
        res.result(http::status::bad_request);
    } else {
        int doc = std::stoi(url.substr(pos + 10));
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT record_id, appointment_date, appointment_time "
            "FROM records WHERE doctor_id=$1::int AND cancelled=TRUE",
            1, nullptr, (const char **)&doc, nullptr, nullptr, 0
        );
        if (!r || PQresultStatus(r) != PGRES_TUPLES_OK) {
            if (r) {
                PQclear(r);
            }
            response["success"] = false;
            response["error"] = "DB error";
            res.result(http::status::internal_server_error);
        } else {
            json arr = json::array();
            for (int i = 0; i < PQntuples(r); ++i) {
                json s;
                s["record_id"] = std::stoi(PQgetvalue(r, i, 0));
                s["date"] = PQgetvalue(r, i, 1);
                s["time"] = PQgetvalue(r, i, 2);
                arr.push_back(std::move(s));
            }
            PQclear(r);
            response["success"] = true;
            response["slots"] = std::move(arr);
            res.result(http::status::ok);
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
