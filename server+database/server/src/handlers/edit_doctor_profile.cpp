#include "../../include/handlers/edit_doctor_profile.hpp"
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <iomanip>
#include <vector>

namespace http = boost::beast::http;
using json = nlohmann::json;

static bool
phone_is_unique(int user_id, const std::string &phone, PGconn *conn) {
    const char *params[2] = {phone.c_str(), std::to_string(user_id).c_str()};
    PGresult *r = PQexecParams(
        conn, "SELECT 1 FROM users WHERE phone = $1 AND id <> $2 LIMIT 1", 2,
        nullptr, params, nullptr, nullptr, 0
    );
    bool exists =
        (r && PQntuples(r) > 0 && PQresultStatus(r) == PGRES_TUPLES_OK);
    if (r) {
        PQclear(r);
    }
    return !exists;
}

void edit_doctor_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    std::vector<std::string> string_storage;  // Для хранения временных строк

    // 1) Проверка обязательных полей
    if (!data.contains("user_id") || !data.contains("current_password")) {
        response["success"] = false;
        response["error"] = "Missing 'user_id' or 'current_password'";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int user_id = data["user_id"].get<int>();
    std::string current_password = data["current_password"];

    // 2) Проверка типа пользователя
    {
        string_storage.push_back(std::to_string(user_id));
        const char *uid_param[1] = {string_storage.back().c_str()};
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_type FROM users WHERE id = $1", 1, nullptr, uid_param,
            nullptr, nullptr, 0
        );

        if (!r || PQresultStatus(r) != PGRES_TUPLES_OK || PQntuples(r) == 0) {
            if (r) {
                PQclear(r);
            }
            response["success"] = false;
            response["error"] = "User not found";
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string ut = PQgetvalue(r, 0, 0);
        PQclear(r);
        if (ut != "doctor") {
            response["success"] = false;
            response["error"] = "Not a doctor";
            res.result(http::status::forbidden);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    // 3) Проверка текущего пароля
    std::string db_hashed, db_salt;
    {
        string_storage.push_back(std::to_string(user_id));
        const char *uid_param[1] = {string_storage.back().c_str()};
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT hashed_password, salt FROM users WHERE id = $1", 1, nullptr,
            uid_param, nullptr, nullptr, 0
        );

        if (!r || PQresultStatus(r) != PGRES_TUPLES_OK || PQntuples(r) == 0) {
            if (r) {
                PQclear(r);
            }
            response["success"] = false;
            response["error"] = "User not found";
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        db_hashed = PQgetvalue(r, 0, 0);
        db_salt = PQgetvalue(r, 0, 1);
        PQclear(r);
    }

    // Хеширование текущего пароля
    std::string to_hash = current_password + db_salt;
    unsigned char hbuf[SHA256_DIGEST_LENGTH];
    SHA256(
        reinterpret_cast<const unsigned char *>(to_hash.c_str()),
        to_hash.size(), hbuf
    );

    std::string current_hash;
    current_hash.reserve(SHA256_DIGEST_LENGTH * 2);
    static const char hex_digits[] = "0123456789abcdef";
    for (unsigned char byte : hbuf) {
        current_hash += hex_digits[(byte >> 4) & 0x0F];
        current_hash += hex_digits[byte & 0x0F];
    }

    if (current_hash != db_hashed) {
        response["success"] = false;
        response["error"] = "Current password is incorrect";
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4) Подготовка параметров для UPDATE
    std::vector<std::string> clauses;
    std::vector<const char *> params;
    int param_index = 1;

    auto add_text_param = [&](const std::string &field,
                              const std::string &value) {
        clauses.push_back(
            field + " = $" + std::to_string(param_index) + "::text"
        );
        string_storage.push_back(value);
        params.push_back(string_storage.back().c_str());
        param_index++;
    };

    if (data.contains("last_name")) {
        add_text_param("last_name", data["last_name"].get<std::string>());
    }
    if (data.contains("first_name")) {
        add_text_param("first_name", data["first_name"].get<std::string>());
    }
    if (data.contains("patronymic")) {
        add_text_param("patronymic", data["patronymic"].get<std::string>());
    }
    if (data.contains("phone")) {
        std::string new_phone = data["phone"].get<std::string>();
        if (!phone_is_unique(user_id, new_phone, db_handler.get_connection())) {
            response["success"] = false;
            response["error"] = "Phone already in use";
            res.result(http::status::conflict);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        add_text_param("phone", new_phone);
    }

    // Обработка смены пароля
    if (data.contains("new_password") || data.contains("new_password_repeat")) {
        if (!data.contains("new_password") ||
            !data.contains("new_password_repeat")) {
            response["success"] = false;
            response["error"] =
                "Both 'new_password' and 'new_password_repeat' must be present";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string np1 = data["new_password"].get<std::string>();
        std::string np2 = data["new_password_repeat"].get<std::string>();
        if (np1 != np2) {
            response["success"] = false;
            response["error"] = "New passwords do not match";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string new_salt = db_handler.generate_salt(16);
        std::string tohash = np1 + new_salt;
        unsigned char new_hash_bytes[SHA256_DIGEST_LENGTH];
        SHA256(
            reinterpret_cast<const unsigned char *>(tohash.c_str()),
            tohash.size(), new_hash_bytes
        );

        std::string new_hash;
        new_hash.reserve(SHA256_DIGEST_LENGTH * 2);
        for (unsigned char byte : new_hash_bytes) {
            new_hash += hex_digits[(byte >> 4) & 0x0F];
            new_hash += hex_digits[byte & 0x0F];
        }

        add_text_param("hashed_password", new_hash);
        add_text_param("salt", new_salt);
    }

    if (clauses.empty()) {
        response["success"] = false;
        response["error"] = "No fields to update";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 5) Выполнение UPDATE
    std::string sql = "UPDATE users SET ";
    for (size_t i = 0; i < clauses.size(); ++i) {
        if (i > 0) {
            sql += ", ";
        }
        sql += clauses[i];
    }
    sql += " WHERE id = $" + std::to_string(param_index) + "::int";
    string_storage.push_back(std::to_string(user_id));
    params.push_back(string_storage.back().c_str());

    PGresult *update_result = PQexecParams(
        db_handler.get_connection(), sql.c_str(),
        static_cast<int>(params.size()), nullptr, params.data(), nullptr,
        nullptr, 0
    );

    if (!update_result || PQresultStatus(update_result) != PGRES_COMMAND_OK) {
        std::string error_msg =
            update_result ? PQerrorMessage(db_handler.get_connection())
                          : "Unknown error";
        if (update_result) {
            PQclear(update_result);
        }
        response["success"] = false;
        response["error"] = "Database error: " + error_msg;
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PQclear(update_result);
    response["success"] = true;
    response["message"] = "Profile updated successfully";
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}