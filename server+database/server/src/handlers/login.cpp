#include "../../include/handlers/login.hpp"
#include "../../include/database.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

// extern database_handler* global_db;  // Объявляем глобальный указатель, определённый в main.cpp

void login(const nlohmann::json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    nlohmann::json response;

    if (!data.contains("phone") || !data.contains("password")) {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Missing phone or password"})";
        return;
    }

    std::string phone = data["phone"];
    std::string password = data["password"];

    std::string login_result = db_handler.login_user(phone, password);
    bool success = !login_result.empty();

    response["success"] = success;
    response["action"] = "login";

    if (success) {
        response["result"] = login_result;
        res.result(http::status::ok);
    } else {
        response["error"] = "Invalid credentials";
        res.result(http::status::unauthorized);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump(); //тут устанавливам возврат джейсона
}
