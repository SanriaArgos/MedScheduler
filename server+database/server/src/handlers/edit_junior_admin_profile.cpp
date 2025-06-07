#include "../../include/handlers/edit_junior_admin_profile.hpp"
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>

namespace http = boost::beast::http;
using json = nlohmann::json;

static bool password_is_correct(int user_id, const std::string &password, PGconn *conn) {
    const char *params[1] = {std::to_string(user_id).c_str()};
    PGresult *res = PQexecParams(
        conn,
        "SELECT hashed_password, salt FROM users WHERE id = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return false;
    }
    
    std::string db_hash = PQgetvalue(res, 0, 0);
    std::string salt = PQgetvalue(res, 0, 1);
    PQclear(res);
    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string to_hash = password + salt;
    SHA256(reinterpret_cast<const unsigned char*>(to_hash.c_str()), to_hash.size(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    
    return ss.str() == db_hash;
}

void edit_junior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    
    // Проверка обязательных полей
    if (!data.contains("user_id") || !data.contains("current_password")) {
        response["success"] = false;
        response["error"] = "Missing required fields";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PGconn* conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        response["success"] = false;
        response["error"] = "Database connection error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Проверка текущего пароля
    if (!password_is_correct(data["user_id"].get<int>(), data["current_password"].get<std::string>(), conn)) {
        response["success"] = false;
        response["error"] = "Invalid current password";
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Подготовка параметров для UPDATE
    std::vector<std::string> clauses;
    std::vector<const char*> params;
    int param_index = 1;

    // Первая лямбда для текстовых параметров
    auto add_text_param = [&](const std::string &field, const std::string &value) {
        clauses.push_back(field + " = $" + std::to_string(param_index) + "::text");
        params.push_back(strdup(value.c_str()));
        param_index++;
    };

    // Обработка полей профиля
    if (data.contains("last_name")) add_text_param("last_name", data["last_name"]);
    if (data.contains("first_name")) add_text_param("first_name", data["first_name"]);
    if (data.contains("patronymic")) add_text_param("patronymic", data["patronymic"]);

    // Обработка смены пароля
    if (data.contains("new_password")) {
        if (!data.contains("new_password_repeat") || 
            data["new_password"] != data["new_password_repeat"]) {
            for (const char* p : params) free((void*)p);
            response["success"] = false;
            response["error"] = "Passwords don't match";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        // Генерация новой соли и хеша
        std::string new_salt = db_handler.generate_salt(16);
        std::string to_hash = data["new_password"].get<std::string>() + new_salt;
        unsigned char new_hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(to_hash.c_str()), to_hash.size(), new_hash);

        // Вторая лямбда (с другим именем) для бинарных параметров
        auto add_binary_param = [&](const std::string &field, const std::string &hex_value) {
            clauses.push_back(field + " = decode($" + std::to_string(param_index) + ", 'hex')");
            params.push_back(strdup(hex_value.c_str()));
            param_index++;
        };

        std::stringstream hash_ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            hash_ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(new_hash[i]);
        }

        std::stringstream salt_ss;
        for (char c : new_salt) {
            salt_ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }

        add_binary_param("hashed_password", hash_ss.str());
        add_binary_param("salt", salt_ss.str());
    }

    // Выполнение UPDATE
    if (!clauses.empty()) {
        std::string sql = "UPDATE users SET ";
        for (size_t i = 0; i < clauses.size(); ++i) {
            if (i > 0) sql += ", ";
            sql += clauses[i];
        }
        sql += " WHERE id = $" + std::to_string(param_index) + "::int";
        params.push_back(strdup(std::to_string(data["user_id"].get<int>()).c_str()));

        PGresult* update_res = PQexecParams(conn, sql.c_str(), params.size(), nullptr, params.data(), nullptr, nullptr, 0);
        
        // Освобождение памяти
        for (const char* p : params) free((void*)p);

        if (PQresultStatus(update_res) != PGRES_COMMAND_OK) {
            PQclear(update_res);
            response["success"] = false;
            response["error"] = "Database update failed";
            res.result(http::status::internal_server_error);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        PQclear(update_res);
    }

    response["success"] = true;
    response["message"] = "Profile updated successfully";
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}