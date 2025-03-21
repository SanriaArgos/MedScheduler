#include "display_users.hpp"
#include <iostream>
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern database_handler* global_db;

json display_users_table(const json &data) {
    json result;
    PGconn *conn = global_db->get_connection();
    PGresult *res = PQexecParams(conn,
        "SELECT id, last_name, first_name, patronymic, phone, user_type FROM users ORDER BY id",
        0, nullptr, nullptr, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        result["success"] = false;
        result["error"] = "Users table unavailable";
        if (res) PQclear(res);
        return result;
    }
    int rows = PQntuples(res);
    json users = json::array();
    for (int i = 0; i < rows; ++i) {
        json user;
        user["id"] = PQgetvalue(res, i, 0);
        user["last_name"] = PQgetvalue(res, i, 1);
        user["first_name"] = PQgetvalue(res, i, 2);
        user["patronymic"] = PQgetvalue(res, i, 3);
        user["phone"] = PQgetvalue(res, i, 4);
        user["user_type"] = PQgetvalue(res, i, 5);
        users.push_back(user);
    }
    PQclear(res);
    result["success"] = true;
    result["users"] = users;
    return result;
}
