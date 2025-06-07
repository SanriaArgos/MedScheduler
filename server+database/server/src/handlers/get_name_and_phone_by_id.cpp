#include "../../include/handlers/get_name_and_phone_by_id.hpp"
#include <libpq-fe.h>

void get_name_and_phone_by_id(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверяем, что пришёл user_id
    if (!data.contains("user_id")) {
        response["success"] = false;
        response["error"]   = "Missing user_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int user_id = data["user_id"];
    std::string user_id_str = std::to_string(user_id);
    const char *params[1] = { user_id_str.c_str() };

    // 2. Выполняем запрос к БД
    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "SELECT last_name, first_name, patronymic, phone "
        "FROM users WHERE id = $1",
        1,       // число параметров
        nullptr, // типы параметров (text)
        params,  // значения параметров
        nullptr, // длины (text)
        nullptr, // форматы (text)
        0        // текстовый режим
    );

    // 3. Обработка ошибок БД
    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) PQclear(pgres);
        response["success"] = false;
        response["error"]   = "Database error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4. Если пользователя нет
    if (PQntuples(pgres) == 0) {
        PQclear(pgres);
        response["success"] = false;
        response["error"]   = "User not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 5. Собираем результат
    json user;
    user["last_name"]   = PQgetvalue(pgres, 0, 0);
    user["first_name"]  = PQgetvalue(pgres, 0, 1);
    user["patronymic"]  = PQgetvalue(pgres, 0, 2);
    user["phone"]       = PQgetvalue(pgres, 0, 3);

    PQclear(pgres);

    response["success"] = true;
    response["user"]    = user;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
