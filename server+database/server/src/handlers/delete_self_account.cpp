#include "../../include/handlers/delete_self_account.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
extern database_handler *global_db;

void delete_self_account(
    int user_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    // Устанавливаем Content-Type один раз в начале
    res.set(http::field::content_type, "application/json");

    // 1) Получаем тип пользователя
    std::string user_type;
    {
        const char *param = std::to_string(user_id).c_str();
        PGresult *result = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_type FROM users WHERE id = $1::int", 
            1, nullptr, &param, nullptr, nullptr, 0
        );

        if (!result || PQntuples(result) == 0) {
            json error_response = {
                {"success", false}, 
                {"error", "User not found"}
            };
            res.result(http::status::not_found);
            res.body() = error_response.dump();
            if (result) PQclear(result);
            return;
        }

        user_type = PQgetvalue(result, 0, 0);
        PQclear(result);
    }

    // 2) Обработка в зависимости от типа
    if (user_type == "patient") {
        const char *patient_param = std::to_string(user_id).c_str();
        
        // Удаляем связанные данные
        PGresult *records_result = PQexecParams(
            db_handler.get_connection(),
            "DELETE FROM records WHERE patient_id = $1::int",
            1, nullptr, &patient_param, nullptr, nullptr, 0
        );
        if (records_result) PQclear(records_result);

        PGresult *waitlist_result = PQexecParams(
            db_handler.get_connection(),
            "DELETE FROM waitlist WHERE patient_id = $1::int",
            1, nullptr, &patient_param, nullptr, nullptr, 0
        );
        if (waitlist_result) PQclear(waitlist_result);

    } else if (user_type == "doctor") {
        // Получаем doctor_id
        int doctor_id = -1;
        const char *user_param = std::to_string(user_id).c_str();
        PGresult *doctor_result = PQexecParams(
            db_handler.get_connection(),
            "SELECT doctor_id FROM doctors WHERE user_id = $1::int",
            1, nullptr, &user_param, nullptr, nullptr, 0
        );

        if (doctor_result && PQntuples(doctor_result) > 0) {
            doctor_id = std::stoi(PQgetvalue(doctor_result, 0, 0));
        }
        if (doctor_result) PQclear(doctor_result);

        if (doctor_id >= 0) {
            const char *doctor_param = std::to_string(doctor_id).c_str();
            
            PGresult *doctor_records = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM records WHERE doctor_id = $1::int",
                1, nullptr, &doctor_param, nullptr, nullptr, 0
            );
            if (doctor_records) PQclear(doctor_records);

            PGresult *doctor_waitlist = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE doctor_id = $1::int",
                1, nullptr, &doctor_param, nullptr, nullptr, 0
            );
            if (doctor_waitlist) PQclear(doctor_waitlist);
        }
    } else {
        json error_response = {
            {"success", false},
            {"error", "Forbidden for user type: " + user_type}
        };
        res.result(http::status::forbidden);
        res.body() = error_response.dump();
        return;
    }

    // 3) Удаляем пользователя
    const char *user_param = std::to_string(user_id).c_str();
    PGresult *delete_result = PQexecParams(
        db_handler.get_connection(),
        "DELETE FROM users WHERE id = $1::int",
        1, nullptr, &user_param, nullptr, nullptr, 0
    );

    if (!delete_result || PQresultStatus(delete_result) != PGRES_COMMAND_OK) {
        json error_response = {
            {"success", false},
            {"error", "Database deletion failed"}
        };
        res.result(http::status::internal_server_error);
        res.body() = error_response.dump();
        if (delete_result) PQclear(delete_result);
        return;
    }

    PQclear(delete_result);

    // 4) Успешный ответ
    json success_response = {{"success", true}};
    res.result(http::status::ok);
    res.body() = success_response.dump();
}