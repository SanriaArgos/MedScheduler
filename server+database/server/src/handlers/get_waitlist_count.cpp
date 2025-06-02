#include "../../include/handlers/get_waitlist_count.hpp"
#include <libpq-fe.h>

void get_waitlist_count(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    const char *p = std::to_string(doctor_id).c_str();
    PGresult *r = PQexecParams(
        db_handler.get_connection(),
        "SELECT COUNT(*) FROM waitlist WHERE doctor_id = $1::int", 1, nullptr,
        &p, nullptr, nullptr, 0
    );
    if (!r || PQresultStatus(r) != PGRES_TUPLES_OK) {
        if (r) {
            PQclear(r);
        }
        response["success"] = false;
        response["error"] = "DB error";
        res.result(http::status::internal_server_error);
    } else {
        int cnt = std::stoi(PQgetvalue(r, 0, 0));
        PQclear(r);
        response["success"] = true;
        response["count"] = cnt;
        res.result(http::status::ok);
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
