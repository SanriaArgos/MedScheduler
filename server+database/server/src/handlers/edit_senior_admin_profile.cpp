#include "../../include/handlers/edit_senior_admin_profile.hpp"
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <vector>
#include <sstream>
#include <memory>

namespace http = boost::beast::http;
using json = nlohmann::json;

static bool phone_is_unique(int user_id, const std::string &phone, PGconn *conn) {
    const char *params[2] = { phone.c_str(), std::to_string(user_id).c_str() };
    PGresult *r = PQexecParams(
        conn,
        "SELECT 1 FROM users WHERE phone = $1 AND id <> $2 LIMIT 1",
        2, nullptr, params, nullptr, nullptr, 0
    );
    bool ok = (r && PQresultStatus(r) == PGRES_TUPLES_OK && PQntuples(r) == 0);
    if (r) PQclear(r);
    return ok;
}
void edit_senior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    auto send_response = [&](http::status status, const std::string &error = "", bool success = false) {
        response["success"] = success;
        if (!success) response["error"] = error;
        res.result(status);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
    };

    if (!data.contains("user_id") || !data.contains("current_password")) {
        return send_response(http::status::bad_request, "Missing user_id or current_password");
    }

    int user_id = data["user_id"];
    std::string current_password = data["current_password"];

    // Получение информации о пользователе
    const char *p1[1] = { std::to_string(user_id).c_str() };
    PGresult *creds = PQexecParams(
        db_handler.get_connection(),
        "SELECT user_type, hashed_password, salt FROM users WHERE id = $1",
        1, nullptr, p1, nullptr, nullptr, 0
    );

    if (!creds || PQresultStatus(creds) != PGRES_TUPLES_OK || PQntuples(creds) == 0) {
        if (creds) PQclear(creds);
        return send_response(http::status::not_found, "User not found");
    }

    std::string user_type = PQgetvalue(creds, 0, 0);
    std::string db_hashed = PQgetvalue(creds, 0, 1);
    std::string db_salt   = PQgetvalue(creds, 0, 2);
    PQclear(creds);

    if (user_type != "senior administrator") {
        return send_response(http::status::forbidden, "User is not a senior administrator");
    }

    // Проверка текущего пароля
    if (db_handler.hash_password(current_password, db_salt) != db_hashed) {
        return send_response(http::status::unauthorized, "Invalid current_password");
    }

    PGconn *conn = db_handler.get_connection();
    std::vector<std::string> clauses;
    std::vector<std::string> raw_params;

    auto add_param = [&](const std::string &field, const std::string &value) {
        clauses.push_back(field + " = $" + std::to_string(raw_params.size() + 1));
        raw_params.push_back(value);
    };

    if (data.contains("last_name"))   add_param("last_name",   data["last_name"]);
    if (data.contains("first_name"))  add_param("first_name",  data["first_name"]);
    if (data.contains("patronymic"))  add_param("patronymic",  data["patronymic"]);

    if (data.contains("phone")) {
        std::string phone = data["phone"];
        if (!phone_is_unique(user_id, phone, conn)) {
            return send_response(http::status::conflict, "Phone already in use");
        }
        add_param("phone", phone);
    }

    // Обновление пароля
    if (data.contains("new_password") || data.contains("new_password_repeat")) {
        if (!data.contains("new_password") || !data.contains("new_password_repeat")) {
            return send_response(http::status::bad_request, "Both new_password and new_password_repeat must be provided");
        }
        std::string np1 = data["new_password"];
        std::string np2 = data["new_password_repeat"];
        if (np1 != np2) {
            return send_response(http::status::bad_request, "New passwords do not match");
        }
        std::string new_salt = db_handler.generate_salt(16);
        std::string new_hash = db_handler.hash_password(np1, new_salt);
        add_param("hashed_password", new_hash);
        add_param("salt", new_salt);
    }

    if (clauses.empty()) {
        return send_response(http::status::bad_request, "No fields to update");
    }

    std::ostringstream query;
    query << "UPDATE users SET ";
    for (size_t i = 0; i < clauses.size(); ++i) {
        if (i > 0) query << ", ";
        query << clauses[i];
    }
    query << " WHERE id = $" << raw_params.size() + 1;
    raw_params.push_back(std::to_string(user_id));

    std::vector<const char*> param_ptrs;
    for (auto &s : raw_params) param_ptrs.push_back(s.c_str());

    PGresult *upd = PQexecParams(
        conn,
        query.str().c_str(),
        (int)param_ptrs.size(), nullptr,
        param_ptrs.data(), nullptr, nullptr, 0
    );

    if (!upd || PQresultStatus(upd) != PGRES_COMMAND_OK) {
        std::string err = PQerrorMessage(conn);
        if (upd) PQclear(upd);
        return send_response(http::status::internal_server_error, "Database error: " + err);
    }

    PQclear(upd);
    return send_response(http::status::ok, "", true);
}
