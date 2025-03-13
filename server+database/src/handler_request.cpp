#include "handler_request.hpp"
#include "nlohmann/json.hpp"
#include "handlers.hpp"
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

namespace http = boost::beast::http;
using json = nlohmann::json;

void handle_request(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    Database_Handler &db_handler
) {
    try {
        std::string token;
        if (req.find(http::field::authorization) != req.end()) {
            auto auth_header = req[http::field::authorization].to_string();
            if (auth_header.find("Bearer ") == 0) {
                token = auth_header.substr(7);
            }
        } //а что здесь будет если токен не найдется? тип где else для этого if?

        if (req.method() == http::verb::post && req.target() == "/login") {
            json body = json::parse(req.body());
            std::string phone = body["phone"];
            std::string password = body["password"];

            std::string user_token = db_handler.login_User(phone, password);
            if (!user_token.empty()) {
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"token": ")" + user_token + R"("})";
            } else {
                res.result(http::status::unauthorized);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "Invalid credentials"})";
            }
        } else if (req.method() == http::verb::post && req.target() == "/registration") {
            json body = json::parse(req.body());
            std::string last_name = body["last_name"];
            std::string first_name = body["first_name"];
            std::string patronymic = body["patronymic"];
            std::string phone = body["phone"];
            std::string password = body["password"];

            if (db_handler.user_Exists(phone)) {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "User already exists"})";
            } else {
                bool success = db_handler.register_User(
                    last_name, first_name, patronymic, phone, password
                );
                if (success) {
                    res.result(http::status::created);
                    res.set(http::field::content_type, "application/json");
                    res.body() =
                        R"({"message": "User registered successfully"})";
                } else {
                    res.result(http::status::internal_server_error);
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"error": "Failed to register user"})";
                }
            }
        } else {
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Route not found"})";
        }
    } catch (const std::exception &e) {
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Internal server error"})";
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    res.prepare_payload(); //для настройки ответа
}