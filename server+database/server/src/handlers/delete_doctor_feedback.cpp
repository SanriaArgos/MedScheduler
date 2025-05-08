// Handler: delete_doctor_rating
// Purpose: Удаляет отзыв по id. Доступно только администраторам.
// Input JSON:
//   { "rating_id":<int>, "admin_id":<int> }
// Output JSON (200 OK):
//   { "success": true }
// Errors:
//   400 Bad Request  – нет rating_id или admin_id
//   404 Not Found    – админ не найден
//   403 Forbidden    – недостаточно прав
//   500 Internal     – ошибка БД

#include "../../include/handlers/delete_doctor_feedback.hpp"
#include <libpq-fe.h>

namespace http = boost::beast::http;
using json = nlohmann::json;

void delete_doctor_feedback(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверяем обязательные поля
    if (!data.contains("rating_id") || !data.contains("admin_id")) {
        response["success"] = false;
        response["error"] = "Missing rating_id or admin_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int rating_id = data["rating_id"];
    int admin_id = data["admin_id"];

    // 2. Проверяем права администратора
    std::string admin_id_str = std::to_string(admin_id);
    const char *auth_params[1] = {admin_id_str.c_str()};
    PGresult *auth_res = PQexecParams(
        db_handler.get_connection(),
        "SELECT user_type FROM users WHERE id = $1", 1, nullptr, auth_params,
        nullptr, nullptr, 0
    );

    if (!auth_res || PQresultStatus(auth_res) != PGRES_TUPLES_OK || PQntuples(auth_res) == 0) {
        if (auth_res) PQclear(auth_res);
        response["success"] = false;
        response["error"] = "Admin not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string user_type = PQgetvalue(auth_res, 0, 0);
    PQclear(auth_res);

    if (user_type != "senior administrator" && user_type != "junior administrator") {
        response["success"] = false;
        response["error"] = "Permission denied";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 3. Проверяем существование отзыва перед удалением
    std::string id_str = std::to_string(rating_id);
    const char *check_params[1] = {id_str.c_str()};
    PGresult *check_res = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM rating WHERE id = $1", 1, nullptr, check_params,
        nullptr, nullptr, 0
    );

    if (!check_res || PQresultStatus(check_res) != PGRES_TUPLES_OK) {
        if (check_res) PQclear(check_res);
        response["success"] = false;
        response["error"] = "Database error checking rating";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    if (PQntuples(check_res) == 0) {
        PQclear(check_res);
        response["success"] = false;
        response["error"] = "Rating not found or already deleted";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(check_res);

    // 4. Удаляем отзыв и проверяем количество удаленных строк
    const char *del_params[1] = {id_str.c_str()};
    PGresult *del_res = PQexecParams(
        db_handler.get_connection(),
        "DELETE FROM rating WHERE id = $1", 1, nullptr, del_params,
        nullptr, nullptr, 0
    );

    if (!del_res || PQresultStatus(del_res) != PGRES_COMMAND_OK) {
        if (del_res) PQclear(del_res);
        response["success"] = false;
        response["error"] = "Error deleting rating";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Проверяем, была ли фактически удалена какая-либо запись
    int rows_affected = atoi(PQcmdTuples(del_res));
    PQclear(del_res);

    if (rows_affected == 0) {
        response["success"] = false;
        response["error"] = "Rating was already deleted";
        res.result(http::status::not_found);
    } else {
        response["success"] = true;
        res.result(http::status::ok);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}