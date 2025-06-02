#include "../../include/handlers/login.hpp"
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"
#include "../../include/handlers/auth_handler.hpp"

namespace http = boost::beast::http;

extern database_handler *global_db;

void login(
    const nlohmann::json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    nlohmann::json response;

    if (!data.contains("phone") || !data.contains("password")) {
        std::cerr << "Login error: missing required fields\n";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Missing phone or password"})";
        return;
    }

    std::string phone = data["phone"];
    std::string password = data["password"];

    std::string login_result = login_user(*global_db, phone, password);
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
    res.body() = response.dump();  // тут устанавливам возврат джейсона
}
