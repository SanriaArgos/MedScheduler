#include "../../include/handlers/registration.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

void registration(const nlohmann::json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    nlohmann::json response;

    if (!data.contains("last_name") || !data.contains("first_name") ||
        !data.contains("phone") || !data.contains("password")) {
        std::cerr << "Registration error: missing required fields\n";
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string last_name = data["last_name"];
    std::string first_name = data["first_name"];
    std::string patronymic = data.value("patronymic", "");
    std::string phone = data["phone"];
    std::string password = data["password"];

    bool success = db_handler.register_user(last_name, first_name, patronymic, phone, password);
    response["success"] = success;
    response["action"] = "registration";

    if (!success) {
        response["error"] = "User already exists or registration failed";
        res.result(http::status::conflict);  // 409
    } else {
        res.result(http::status::ok);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
