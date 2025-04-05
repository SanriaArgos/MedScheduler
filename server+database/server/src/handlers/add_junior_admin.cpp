#include "../../include/handlers/add_junior_admin.hpp"
#include <iostream>
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void add_junior_admin(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    nlohmann::json response;

    // Проверка на наличие обязательных полей
    if (!data.contains("last_name") || !data.contains("first_name") || !data.contains("phone")) {
        std::cerr << "Error: Missing required fields for adding junior admin\n";
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string last_name = data["last_name"];
    std::string first_name = data["first_name"];
    std::string patronymic = data.value("patronymic", "");
    std::string phone = data["phone"];

    // Проверка, если пользователь с таким номером телефона уже существует
    if (global_db->user_exists(phone)) {
        std::cerr << "Error: Phone already registered\n";
        response["success"] = false;
        response["error"] = "Phone already registered";

        res.result(http::status::conflict);  // 409 Conflict
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    const std::string default_password = "0987654321";
    // Регистрация пользователя с дефолтным паролем
    if (!global_db->register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cerr << "Error adding junior administrator\n";
        response["success"] = false;
        response["error"] = "Error adding junior administrator";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Обновление типа пользователя на 'junior administrator'
    const char* params[1] = { phone.c_str() };
    PGresult *res_update = PQexecParams(db_handler.get_connection(),
        "UPDATE users SET user_type = 'junior administrator' WHERE phone = $1",
        1, NULL, params, NULL, NULL, 0);
    
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating user type: " << PQerrorMessage(db_handler.get_connection()) << "\n";
        PQclear(res_update);
        response["success"] = false;
        response["error"] = "Error updating user type";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    
    PQclear(res_update);

    std::cerr << "Junior administrator added\n";
    response["success"] = true;
    response["message"] = "Junior administrator successfully added";

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
