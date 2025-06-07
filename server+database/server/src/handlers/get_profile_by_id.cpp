#include "../../include/handlers/get_profile_by_id.hpp"
#include <libpq-fe.h>

json get_profile_by_id(
    int user_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    PGconn* conn = db_handler.get_connection();

    // 1) Проверка соединения с БД
    if (PQstatus(conn) != CONNECTION_OK) {
        response["success"] = false;
        response["error"] = "Database connection error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return response;
    }

    // 2) Выполнение запроса
    const char *params[1] = {std::to_string(user_id).c_str()};
    PGresult *pgres = PQexecParams(
        conn,
        "SELECT last_name, first_name, patronymic, phone "
        "FROM users WHERE id = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );

    // 3) Обработка ошибок БД
    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) PQclear(pgres);
        response["success"] = false;
        response["error"] = "Database query error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return response;
    }

    // 4) Проверка наличия пользователя
    if (PQntuples(pgres) == 0) {
        PQclear(pgres);
        response["success"] = false;
        response["error"] = "User not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return response;
    }

    // 5) Формирование ответа
    json user;
    user["last_name"] = PQgetvalue(pgres, 0, 0);
    user["first_name"] = PQgetvalue(pgres, 0, 1);
    user["patronymic"] = PQgetvalue(pgres, 0, 2);
    user["phone"] = PQgetvalue(pgres, 0, 3);
    PQclear(pgres);

    response["success"] = true;
    response["user"] = user;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();

    return response;
}