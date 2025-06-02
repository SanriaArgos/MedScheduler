#include "../../include/handlers/cancel_appointment.hpp"
#include <libpq-fe.h>
#include <iostream>

void cancel_appointment(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    
    // 1. Проверка входных данных
    if (!data.contains("record_id") || !data.contains("patient_id")) {
        response["success"] = false;
        response["error"] = "Missing record_id or patient_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 2. Получение параметров
    int record_id = data["record_id"].get<int>();
    int patient_id = data["patient_id"].get<int>();
    
    // 3. Проверка подключения к БД
    if (PQstatus(db_handler.get_connection()) != CONNECTION_OK) {
        response["success"] = false;
        response["error"] = "Database connection error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    try {
        // 4. Проверка очереди waitlist
        bool queue_empty = true;
        PGresult* wl = PQexec(db_handler.get_connection(), "SELECT COUNT(*) FROM waitlist");
        if (wl && PQresultStatus(wl) == PGRES_TUPLES_OK && PQntuples(wl) > 0) {
            queue_empty = (std::stoi(PQgetvalue(wl, 0, 0)) == 0);
        }
        if (wl) PQclear(wl);

        // 5. Формирование SQL запроса
        std::string sql;
        if (queue_empty) {
            sql = "UPDATE records SET patient_id = NULL WHERE record_id = " + 
                  std::to_string(record_id) + " AND patient_id = " + 
                  std::to_string(patient_id);
        } else {
            sql = "UPDATE records SET cancelled = TRUE, cancelled_at = NOW() WHERE record_id = " + 
                  std::to_string(record_id) + " AND patient_id = " + 
                  std::to_string(patient_id);
        }

        // 6. Логирование запроса (для отладки)
        std::cerr << "Executing SQL: " << sql << "\n";

        // 7. Выполнение запроса
        PGresult* r = PQexec(db_handler.get_connection(), sql.c_str());

        // 8. Проверка результата
        if (!r || PQresultStatus(r) != PGRES_COMMAND_OK) {
            response["success"] = false;
            response["error"] = r ? PQresultErrorMessage(r) : "Null result from database";
            res.result(http::status::internal_server_error);
            if (r) PQclear(r);
        } else {
            int affected_rows = atoi(PQcmdTuples(r));
            PQclear(r);
            
            if (affected_rows > 0) {
                response["success"] = true;
                response["message"] = queue_empty ? 
                    "Appointment cancelled" : "Cancelled and notified waitlister";
                response["affected_rows"] = affected_rows;
                res.result(http::status::ok);
            } else {
                response["success"] = false;
                response["error"] = "No matching record found or already updated";
                res.result(http::status::not_found);
            }
        }
    } catch (const std::exception& e) {
        response["success"] = false;
        response["error"] = std::string("Exception: ") + e.what();
        res.result(http::status::internal_server_error);
    }

    // 9. Отправка ответа
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}