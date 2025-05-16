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
        // Параметры для SQL: record_id и patient_id
        int record_id = data["record_id"].get<int>();
        int patient_id = data["patient_id"].get<int>();
        const char *p[2] = {
            std::to_string(record_id).c_str(),
            std::to_string(patient_id).c_str()
        };
        // Сначала проверяем: есть ли кто-то в очереди (waitlist)?
        PGresult *wl = PQexec(
            db_handler.get_connection(),
            "SELECT COUNT(*) FROM waitlist"
        );
        bool queue_empty = true;
        if (wl && PQresultStatus(wl) == PGRES_TUPLES_OK && PQntuples(wl) > 0) {
            int cnt = std::stoi(PQgetvalue(wl, 0, 0));
            queue_empty = (cnt == 0);
        }
        if (wl) PQclear(wl);

        PGresult *r = nullptr;
        if (queue_empty) {
            // Если очередь пуста — просто освобождаем слот (удаляем patient_id)
            r = PQexecParams(
                db_handler.get_connection(),
                "UPDATE records "
                "   SET patient_id = NULL "
                " WHERE record_id = $1::int "
                "   AND patient_id = $2::int",
                2, nullptr, p, nullptr, nullptr, 0
            );
        } else {
            // Иначе — стандартная отмена с флагом cancelled (и триггером уведомления)
            r = PQexecParams(
                db_handler.get_connection(),
                "UPDATE records "
                "   SET cancelled = TRUE, cancelled_at = NOW() "
                " WHERE record_id = $1::int "
                "   AND patient_id = $2::int",
                2, nullptr, p, nullptr, nullptr, 0
            );
        }

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
