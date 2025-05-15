#include "../../include/handlers/delete_user_by_phone.hpp"
#include "../../include/get_hospital_id.hpp"
#include "../../include/check_doctor_hospital.hpp"

#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler* global_db;

void delete_user_by_phone(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1) Валидация входных полей
    if (!data.contains("phone") || !data.contains("admin_id")) {
        response["success"] = false;
        response["error"]   = "Missing phone or admin_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string target_phone = data["phone"].get<std::string>();
    int admin_id             = data["admin_id"].get<int>();

    // 2) Проверяем, что вызывающий — senior administrator
    {
        const char *p_admin = std::to_string(admin_id).c_str();
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_type FROM users WHERE id = $1::int",
            1, nullptr, &p_admin, nullptr, nullptr, 0
        );
        if (!r || PQntuples(r) == 0) {
            response["success"] = false;
            response["error"]   = "Admin not found";
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            if (r) PQclear(r);
            res.body() = response.dump();
            return;
        }
        std::string admin_type = PQgetvalue(r, 0, 0);
        PQclear(r);
        if (admin_type != "senior administrator") {
            response["success"] = false;
            response["error"]   = "Forbidden: only senior admins";
            res.result(http::status::forbidden);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    // 3) Находим пользователя по телефону
    int user_id;
    std::string user_type;
    {
        const char *p_phone = target_phone.c_str();
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT id, user_type FROM users WHERE phone = $1",
            1, nullptr, &p_phone, nullptr, nullptr, 0
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
        user_id   = std::stoi(PQgetvalue(r, 0, 0));
        user_type = PQgetvalue(r, 0, 1);
        PQclear(r);
    }

    // 4) Удаляем связанные данные в зависимости от типа пользователя
    if (user_type == "patient") {
        // 4.1) Удаляем все записи пациента
        {
            const char *p = std::to_string(user_id).c_str();
            PGresult *r = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM records WHERE patient_id = $1::int",
                1, nullptr, &p, nullptr, nullptr, 0
            );
            if (r) PQclear(r);
        }
        {
            const char *p = std::to_string(user_id).c_str();
            PGresult *r = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE patient_id = $1::int",
                1, nullptr, &p, nullptr, nullptr, 0
            );
            if (r) PQclear(r);
        }

    } else if (user_type == "junior administrator") {
        // 4.2) Удаляем его больницу и убираем hospital_id из списка врачей
        int hospital_id = get_hospital_id_admin(user_id);
        if (hospital_id > 0) {
            // удалить саму больницу
            {
                const char *p = std::to_string(hospital_id).c_str();
                PGresult *r = PQexecParams(
                    db_handler.get_connection(),
                    "DELETE FROM hospitals WHERE hospital_id = $1::int",
                    1, nullptr, &p, nullptr, nullptr, 0
                );
                if (r) PQclear(r);
            }
            // удалить hospital_id из массива hospital_ids у всех врачей
            {
                const char *p = std::to_string(hospital_id).c_str();
                PGresult *r = PQexecParams(
                    db_handler.get_connection(),
                    "UPDATE doctors "
                    "SET hospital_ids = array_remove(hospital_ids, $1::int) "
                    "WHERE $1::int = ANY(hospital_ids)",
                    1, nullptr, &p, nullptr, nullptr, 0
                );
                if (r) PQclear(r);
            }
        }

    } else if (user_type == "doctor") {
        // 4.3) Удаляем doctor_id-связанные таблицы
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
            PGresult *r1 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM records WHERE doctor_id = $1::int",
                1, nullptr, &pdoc, nullptr, nullptr, 0
            );
            if (r1) PQclear(r1);
            PGresult *r2 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE doctor_id = $1::int",
                1, nullptr, &pdoc, nullptr, nullptr, 0
            );
            if (r2) PQclear(r2);
        }
    }
    // note: senior administrators themselves нельзя удалять через этот метод

    // 5) Удаляем запись из users
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

    // 6) Успешный ответ
    response["success"] = true;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
