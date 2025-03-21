#include "../../include/handlers/display_users.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

json display_users_table(database_handler &db) {
    PGconn *conn = db.get_connection();
    json users_json = json::array(); // Создаем JSON-массив для хранения данных о пользователях

    std::string query = "SELECT id, last_name, first_name, patronymic, phone, user_type FROM users ORDER BY id";
    PGresult *res = PQexec(conn, query.c_str());

    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Ошибка вывода таблицы пользователей: " << PQerrorMessage(conn) << "\n";
        if (res) PQclear(res);
        return json{{"error", "Ошибка при выполнении запроса"}}; // Возвращаем JSON с ошибкой
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        json user = {
            {"id", PQgetvalue(res, i, 0)},
            {"last_name", PQgetvalue(res, i, 1)},
            {"first_name", PQgetvalue(res, i, 2)},
            {"patronymic", PQgetvalue(res, i, 3)},
            {"phone", PQgetvalue(res, i, 4)},
            {"user_type", PQgetvalue(res, i, 5)}
        };
        users_json.push_back(user); // Добавляем данные о пользователе в JSON-массив
    }

    PQclear(res);
    return users_json; // Возвращаем JSON с данными о пользователях
}