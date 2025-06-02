#include "../../include/handlers/cancel_waitlist.hpp"
#include <libpq-fe.h>

void cancel_waitlist(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    if (!data.contains("waitlist_id") || !data.contains("patient_id")) {
        response["success"] = false;
        response["error"] = "Missing waitlist_id or patient_id";
        res.result(http::status::bad_request);
    } else {
        int wl_id = data["waitlist_id"].get<int>();
        int pid = data["patient_id"].get<int>();

        // 1) проверяем принадлежность
        const char *check_p[2] = {
            std::to_string(wl_id).c_str(), std::to_string(pid).c_str()};
        PGresult *chk = PQexecParams(
            db_handler.get_connection(),
            "SELECT 1 FROM waitlist WHERE id = $1::int AND patient_id = "
            "$2::int",
            2, nullptr, check_p, nullptr, nullptr, 0
        );
        bool ok =
            (chk && PQresultStatus(chk) == PGRES_TUPLES_OK && PQntuples(chk) > 0
            );
        PQclear(chk);

        if (!ok) {
            response["success"] = false;
            response["error"] = "Not found or not yours";
            res.result(http::status::forbidden);
        } else {
            // 2) удаляем запись
            PGresult *del = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE id = $1::int", 1, nullptr, check_p,
                nullptr, nullptr, 0
            );
            if (del && PQresultStatus(del) == PGRES_COMMAND_OK) {
                response["success"] = true;
                response["message"] = "Removed from waitlist";
                res.result(http::status::ok);
            } else {
                response["success"] = false;
                response["error"] = "Delete failed";
                res.result(http::status::internal_server_error);
            }
            PQclear(del);
        }
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
