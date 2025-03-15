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
#include <boost/algorithm/string.hpp> 

namespace http = boost::beast::http;
using json = nlohmann::json;

std::string base64_decode(const std::string &encoded) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    std::vector<int> vec(256, -1);
    for (int i = 0; i < 64; i++) vec[base64_chars[i]] = i;

    int val = 0, bits = -8;
    for (unsigned char c : encoded) {
        if (vec[c] == -1) break;
        val = (val << 6) + vec[c];
        bits += 6;
        if (bits >= 0) {
            decoded.push_back(char((val >> bits) & 0xFF));
            bits -= 8;
            val &= 0xFFF;
        }
    }
    return decoded;
}

bool extract_basic_credentials(const std::string &auth_header, std::string &phone, std::string &password) {
    if (auth_header.find("Basic ") == 0) {
        std::string encoded = auth_header.substr(6); // Убираем "Basic "
        std::string decoded = base64_decode(encoded);
        size_t separator = decoded.find(':');
        if (separator != std::string::npos) {
            phone = decoded.substr(0, separator);
            password = decoded.substr(separator + 1);
            return true;
        }
    }
    return false;
}

void handle_login(const json &body, http::response<http::string_body> &res, database_handler &db_handler) {
    std::string phone = body["phone"];
    std::string password = body["password"];

    std::string user_token = db_handler.login_user(phone, password);
    if (!user_token.empty()) {
        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"token": ")" + user_token + R"("})";
    } else {
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Invalid credentials"})";
    }
}

void handle_registration(const json &body, http::response<http::string_body> &res, database_handler &db_handler) {
    std::string last_name = body["last_name"];
    std::string first_name = body["first_name"];
    std::string patronymic = body["patronymic"];
    std::string phone = body["phone"];
    std::string password = body["password"];

    if (db_handler.user_exists(phone)) {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "User already exists"})";
    } else {
        bool success = db_handler.register_user(last_name, first_name, patronymic, phone, password);
        if (success) {
            res.result(http::status::created);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"message": "User registered successfully"})";
        } else {
            res.result(http::status::internal_server_error);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Failed to register user"})";
        }
    }
}

void handle_not_found(http::response<http::string_body> &res) {
    res.result(http::status::not_found);
    res.set(http::field::content_type, "application/json");
    res.body() = R"({"error": "Route not found"})";
}

void handle_error(const std::exception &e, http::response<http::string_body> &res) {
    res.result(http::status::internal_server_error);
    res.set(http::field::content_type, "application/json");
    res.body() = R"({"error": "Internal server error", "details": ")" + std::string(e.what()) + R"("})";
    std::cerr << "Exception: " << e.what() << std::endl;
}

void handle_request(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    try {
        std::string phone, password;
        if (req.find(http::field::authorization) != req.end()) {
            auto auth_header = req[http::field::authorization].to_string();
            if (!extract_basic_credentials(auth_header, phone, password)) {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "Invalid Basic Authentication header"})";
                return;
            }
        }

        if (phone.empty() && req.target() != "/login" && req.target() != "/registration") {
            res.result(http::status::unauthorized);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Authentication required"})";
            return;
        }

        if (req.method() == http::verb::post) {
            if (req.target() == "/login") {
                handle_login(json::parse(req.body()), res, db_handler);
            } else if (req.target() == "/registration") {
                handle_registration(json::parse(req.body()), res, db_handler);
            } else {
                handle_not_found(res);
            }
        } else {
            res.result(http::status::method_not_allowed);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Method not allowed"})";
        }
    } catch (const std::exception &e) {
        handle_error(e, res);
    }
    res.prepare_payload();
}