#include "../../include/handlers/delete_user_by_phone.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"
// include "../../include/check_doctor_hospital.hpp"
#include "../../include/get_hospital_id.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void delete_user_by_phone(
    const std::string &phone,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    res.set(http::field::content_type, "application/json");

    // 1. Находим пользователя по телефону
    int user_id = -1;
    std::string user_type;
    {
        const char *param = phone.c_str();
        PGresult *result = PQexecParams(
            db_handler.get_connection(),
            "SELECT id, user_type FROM users WHERE phone = $1", 1, nullptr,
            &param, nullptr, nullptr, 0
        );

        if (!result || PQntuples(result) == 0) {
            response = {{"success", false}, {"error", "User not found"}};
            res.result(http::status::not_found);
            res.body() = response.dump();
            if (result) {
                PQclear(result);
            }
            return;
        }

        user_id = std::stoi(PQgetvalue(result, 0, 0));
        user_type = PQgetvalue(result, 0, 1);
        PQclear(result);
    }

    // 2. Удаляем связанные данные
    if (user_type == "patient") {
        // Удаление записей пациента
        const char *patient_param = std::to_string(user_id).c_str();
        PGresult *r1 = PQexecParams(
            db_handler.get_connection(),
            "DELETE FROM records WHERE patient_id = $1::int", 1, nullptr,
            &patient_param, nullptr, nullptr, 0
        );
        if (r1) {
            PQclear(r1);
        }

        PGresult *r2 = PQexecParams(
            db_handler.get_connection(),
            "DELETE FROM waitlist WHERE patient_id = $1::int", 1, nullptr,
            &patient_param, nullptr, nullptr, 0
        );
        if (r2) {
            PQclear(r2);
        }
    } else if (user_type == "junior administrator") {
        // Удаление больницы и связей
        int hospital_id = get_hospital_id_admin(user_id);
        if (hospital_id > 0) {
            const char *hosp_param = std::to_string(hospital_id).c_str();

            PGresult *r1 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM hospitals WHERE hospital_id = $1::int", 1, nullptr,
                &hosp_param, nullptr, nullptr, 0
            );
            if (r1) {
                PQclear(r1);
            }

            PGresult *r2 = PQexecParams(
                db_handler.get_connection(),
                "UPDATE doctors SET hospital_ids = array_remove(hospital_ids, "
                "$1::int) "
                "WHERE $1::int = ANY(hospital_ids)",
                1, nullptr, &hosp_param, nullptr, nullptr, 0
            );
            if (r2) {
                PQclear(r2);
            }
        }
    } else if (user_type == "doctor") {
        // Удаление данных врача
        const char *user_param = std::to_string(user_id).c_str();
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT doctor_id FROM doctors WHERE user_id = $1::int", 1, nullptr,
            &user_param, nullptr, nullptr, 0
        );

        if (r && PQntuples(r) > 0) {
            int doctor_id = std::stoi(PQgetvalue(r, 0, 0));
            const char *doc_param = std::to_string(doctor_id).c_str();

            PGresult *r1 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM records WHERE doctor_id = $1::int", 1, nullptr,
                &doc_param, nullptr, nullptr, 0
            );
            if (r1) {
                PQclear(r1);
            }

            PGresult *r2 = PQexecParams(
                db_handler.get_connection(),
                "DELETE FROM waitlist WHERE doctor_id = $1::int", 1, nullptr,
                &doc_param, nullptr, nullptr, 0
            );
            if (r2) {
                PQclear(r2);
            }
        }
        if (r) {
            PQclear(r);
        }
    }

    // 3. Удаляем самого пользователя
    const char *user_param = std::to_string(user_id).c_str();
    PGresult *delete_result = PQexecParams(
        db_handler.get_connection(), "DELETE FROM users WHERE id = $1::int", 1,
        nullptr, &user_param, nullptr, nullptr, 0
    );

    if (!delete_result || PQresultStatus(delete_result) != PGRES_COMMAND_OK) {
        response = {
            {"success", false},
            {"error", "Failed to delete user from database"}};
        res.result(http::status::internal_server_error);
        if (delete_result) {
            PQclear(delete_result);
        }
        res.body() = response.dump();
        return;
    }

    PQclear(delete_result);

    // 4. Успешный ответ
    response["success"] = true;
    res.result(http::status::ok);
    res.body() = response.dump();
}