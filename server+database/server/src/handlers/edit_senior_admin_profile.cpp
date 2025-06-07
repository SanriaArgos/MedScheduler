#include "../../include/handlers/edit_senior_admin_profile.hpp"
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

std::string bytes_to_hex_senior(const unsigned char *data, size_t len) {
    std::string hex;
    hex.reserve(len * 2);
    static const char hex_digits[] = "0123456789abcdef";
    for (size_t i = 0; i < len; ++i) {
        hex.push_back(hex_digits[(data[i] >> 4) & 0xF]);
        hex.push_back(hex_digits[data[i] & 0xF]);
    }
    return hex;
}
void edit_senior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    if (!data.contains("user_id") || !data.contains("current_password")) {
        response["success"] = false;
        response["error"] = "Missing 'user_id' or 'current_password'";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PGconn *conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        response["success"] = false;
        response["error"] = "Database connection failed";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int user_id = 0;
    try {
        user_id = data["user_id"].get<int>();
    } catch (...) {
        response["success"] = false;
        response["error"] = "Invalid user_id format";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string current_password = data["current_password"].get<std::string>();

    const char *uid_param = std::to_string(user_id).c_str();
    PGresult *type_result = PQexecParams(
        conn, "SELECT user_type FROM users WHERE id = $1", 1, nullptr,
        &uid_param, nullptr, nullptr, 0
    );

    if (PQresultStatus(type_result) != PGRES_TUPLES_OK || PQntuples(type_result) == 0) {
        PQclear(type_result);
        response["success"] = false;
        response["error"] = "User not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string user_type = PQgetvalue(type_result, 0, 0);
    PQclear(type_result);

    if (user_type != "senior administrator") {
        response["success"] = false;
        response["error"] = "Not a senior administrator";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PGresult *creds_result = PQexecParams(
        conn, "SELECT hashed_password, salt FROM users WHERE id = $1", 1,
        nullptr, &uid_param, nullptr, nullptr, 0
    );

    if (PQresultStatus(creds_result) != PGRES_TUPLES_OK) {
        PQclear(creds_result);
        response["success"] = false;
        response["error"] = "Database error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string db_hashed = PQgetvalue(creds_result, 0, 0);
    std::string db_salt = PQgetvalue(creds_result, 0, 1);
    PQclear(creds_result);

    std::string to_hash = current_password + db_salt;
    unsigned char hbuf[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(to_hash.c_str()), to_hash.size(), hbuf);
    std::string current_hash = bytes_to_hex_senior(hbuf, SHA256_DIGEST_LENGTH);

    if (current_hash != db_hashed) {
        response["success"] = false;
        response["error"] = "Current password is incorrect";
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::vector<std::string> clauses;
    std::vector<const char *> params;
    int param_index = 1;

    // Лямбда для добавления текстовых параметров
    auto add_text_param = [&](const std::string &field, const std::string &value) {
        clauses.push_back(field + " = $" + std::to_string(param_index) + "::text");
        params.push_back(strdup(value.c_str()));
        param_index++;
    };

    // Лямбда для добавления бинарных параметров (hex)
    auto add_binary_param = [&](const std::string &field, const std::string &hex_value) {
        clauses.push_back(field + " = decode($" + std::to_string(param_index) + ", 'hex')");
        params.push_back(strdup(hex_value.c_str()));
        param_index++;
    };

    if (data.contains("last_name")) {
        add_text_param("last_name", data["last_name"]);
    }
    if (data.contains("first_name")) {
        add_text_param("first_name", data["first_name"]);
    }
    if (data.contains("patronymic")) {
        add_text_param("patronymic", data["patronymic"]);
    }

    if (data.contains("phone")) {
        std::string phone = data["phone"];
        const char *phone_params[2] = { phone.c_str(), std::to_string(user_id).c_str() };
        PGresult *phone_check = PQexecParams(
            conn,
            "SELECT 1 FROM users WHERE phone = $1 AND id <> $2 LIMIT 1",
            2, nullptr, phone_params, nullptr, nullptr, 0
        );
        bool phone_exists = (phone_check && PQntuples(phone_check) > 0);
        if (phone_check) {
            PQclear(phone_check);
        }
        if (phone_exists) {
            for (const char *p : params) free((void *)p);
            response["success"] = false;
            response["error"] = "Phone already in use";
            res.result(http::status::conflict);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        add_text_param("phone", phone);
    }

    if (data.contains("new_password")) {
        if (!data.contains("new_password_repeat")) {
            for (const char *p : params) free((void *)p);
            response["success"] = false;
            response["error"] = "Both 'new_password' and 'new_password_repeat' must be present";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string new_pass = data["new_password"];
        std::string repeat_pass = data["new_password_repeat"];

        if (new_pass != repeat_pass) {
            for (const char *p : params) free((void *)p);
            response["success"] = false;
            response["error"] = "New passwords do not match";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string new_salt = db_handler.generate_salt(16);
        if (new_salt.empty()) {
            for (const char *p : params) free((void *)p);
            response["success"] = false;
            response["error"] = "Failed to generate salt";
            res.result(http::status::internal_server_error);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string to_hash = new_pass + new_salt;
        unsigned char new_hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char *>(to_hash.c_str()), to_hash.size(), new_hash);

        add_binary_param("hashed_password", bytes_to_hex_senior(new_hash, SHA256_DIGEST_LENGTH));
        add_binary_param("salt", bytes_to_hex_senior(reinterpret_cast<const unsigned char *>(new_salt.data()), new_salt.size()));
    }

    if (clauses.empty()) {
        for (const char *p : params) free((void *)p);
        response["success"] = false;
        response["error"] = "No fields to update";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string sql = "UPDATE users SET ";
    for (size_t i = 0; i < clauses.size(); ++i) {
        if (i > 0) sql += ", ";
        sql += clauses[i];
    }
    sql += " WHERE id = $" + std::to_string(param_index) + "::int";
    params.push_back(strdup(std::to_string(user_id).c_str()));

    if (PQstatus(conn) != CONNECTION_OK) {
        for (const char *p : params) free((void *)p);
        response["success"] = false;
        response["error"] = "Database connection lost";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PGresult *update_result = PQexecParams(conn, sql.c_str(), params.size(), nullptr, params.data(), nullptr, nullptr, 0);

    for (const char *p : params) free((void *)p);

    if (!update_result || PQresultStatus(update_result) != PGRES_COMMAND_OK) {
        std::string error_msg = update_result ? PQerrorMessage(conn) : "Unknown error";
        if (update_result) PQclear(update_result);
        response["success"] = false;
        response["error"] = "Failed to update user data. DB error: " + error_msg;
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PQclear(update_result);

    response["success"] = true;
    response["message"] = "Senior admin profile updated successfully";
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
