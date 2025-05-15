#include "../../include/handlers/delete_self_account.hpp"
#include "../../include/database.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler* global_db;

void delete_self_account(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1) Проверяем входные данные
    if (!data.contains("user_id")) {
        response["success"] = false;
        response["error"]   = "Missing user_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int user_id = data["user_id"].get<int>();

    // 2) Узнаём тип пользователя
    std::string user_type;
    {
        const char *p = std::to_string(user_id).c_str();
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_type FROM users WHERE id = $1::int",
            1, nullptr, &p, nullptr, nullptr, 0
        );
        if (!r || PQntuples(r) == 0) {
            response["success"] = false;
            response["error"]   = "User not found";
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            if (r) PQclear(r);
            res.body() = response.dump();
            return;
        }
        user_type = PQgetvalue(r, 0, 0);
        PQclear(r);
    }

    // 3) В зависимости от типа удаляем связанные записи
    if (user_type == "patient") {
        // Удаляем все записи пациента
        {
            const char *p = std::to_string(user_id).c_str();
            PGresult *r = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM records WHERE patient_id = $1::int",
                1, nullptr, &p, nullptr, nullptr, 0
            );
            if (r) PQclear(r);
        }
        // Удаляем из очереди ожидания
        {
            const char *p = std::to_string(user_id).c_str();
            PGresult *r = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE patient_id = $1::int",
                1, nullptr, &p, nullptr, nullptr, 0
            );
            if (r) PQclear(r);
        }

    } else if (user_type == "doctor") {
        // Сначала находим doctor_id по user_id
        int doctor_id = -1;
        {
            const char *p = std::to_string(user_id).c_str();
            PGresult *r = PQexecParams(
                db_handler.get_connection(),
                "SELECT doctor_id FROM doctors WHERE user_id = $1::int",
                1, nullptr, &p, nullptr, nullptr, 0
            );
            if (r && PQntuples(r) > 0) {
                doctor_id = std::stoi(PQgetvalue(r, 0, 0));
            }
            if (r) PQclear(r);
        }
        if (doctor_id >= 0) {
            const char *pdoc = std::to_string(doctor_id).c_str();
            // Удаляем все записи этого врача
            PGresult *r1 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM records WHERE doctor_id = $1::int",
                1, nullptr, &pdoc, nullptr, nullptr, 0
            );
            if (r1) PQclear(r1);
            // Удаляем заявки на ожидание для этого врача
            PGresult *r2 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE doctor_id = $1::int",
                1, nullptr, &pdoc, nullptr, nullptr, 0
            );
            if (r2) PQclear(r2);
        }
        // Замечание: сама таблица doctors имеет FOREIGN KEY ON DELETE CASCADE на users,
        // так что удаление пользователя удалит строку из doctors.

    } else {
        // Другие типы (junior/senior) не могут удалить свой аккаунт через этот маршрут
        response["success"] = false;
        response["error"]   = "Forbidden for user type";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4) Удаляем запись из users
    {
        const char *p = std::to_string(user_id).c_str();
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "DELETE FROM users WHERE id = $1::int",
            1, nullptr, &p, nullptr, nullptr, 0
        );
        if (!r || PQresultStatus(r) != PGRES_COMMAND_OK) {
            if (r) PQclear(r);
            response["success"] = false;
            response["error"]   = "Failed to delete user";
            res.result(http::status::internal_server_error);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        PQclear(r);
    }

    // 5) Успешно
    response["success"] = true;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
