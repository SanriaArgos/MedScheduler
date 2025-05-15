#include "../../include/handlers/cancel_appointment.hpp"
#include <libpq-fe.h>

void cancel_appointment(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    if (!data.contains("record_id") || !data.contains("patient_id")) {
        response["success"] = false;
        response["error"] = "Missing record_id or patient_id";
        res.result(http::status::bad_request);
    } else {
        const char *p[2] = {
            std::to_string(data["record_id"].get<int>()).c_str(),
            std::to_string(data["patient_id"].get<int>()).c_str()};
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "UPDATE records SET cancelled = TRUE, cancelled_at = NOW() "
            " WHERE record_id = $1::int AND patient_id = $2::int",
            2, nullptr, p, nullptr, nullptr, 0
        );
        if (r && PQresultStatus(r) == PGRES_COMMAND_OK) {
            response["success"] = true;
            response["message"] = "Cancelled and notified waitlister";
            // сам notify_next_waitlister сработает по триггеру
            res.result(http::status::ok);
        } else {
            response["success"] = false;
            response["error"] = "Cancel failed";
            res.result(http::status::internal_server_error);
        }
        if (r) {
            PQclear(r);
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
