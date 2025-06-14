#include "../../include/handlers/edit_junior_admin_profile.hpp"
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <vector>
#include <sstream>
#include <string>

namespace http = boost::beast::http;
using json = nlohmann::json;

void edit_junior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    if (!data.contains("user_id") || !data.contains("current_password")) {
        response["success"] = false;
        response["error"] = "Missing user_id or current_password";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int user_id = data["user_id"];
    std::string current_password = data["current_password"];
    std::string user_id_str = std::to_string(user_id);
    const char *p1[1] = { user_id_str.c_str() };

    PGconn *conn = db_handler.get_connection();
    PGresult *creds = PQexecParams(
        conn,
        "SELECT user_type, hashed_password, salt FROM users WHERE id = $1",
        1, nullptr, p1, nullptr, nullptr, 0
    );

    if (!creds || PQresultStatus(creds) != PGRES_TUPLES_OK || PQntuples(creds) == 0) {
        if (creds) PQclear(creds);
        response["success"] = false;
        response["error"] = "User not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string user_type = PQgetvalue(creds, 0, 0);
    std::string db_hashed = PQgetvalue(creds, 0, 1);
    std::string db_salt   = PQgetvalue(creds, 0, 2);
    PQclear(creds);

    if (user_type != "junior administrator") {
        response["success"] = false;
        response["error"] = "User is not a junior administrator";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    if (db_handler.hash_password(current_password, db_salt) != db_hashed) {
        response["success"] = false;
        response["error"] = "Invalid current_password";
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::vector<std::string> clauses;
    std::vector<std::string> param_values;

    auto add_param = [&](const std::string &field, const std::string &value) {
        clauses.emplace_back(field + " = $" + std::to_string(param_values.size() + 1));
        param_values.push_back(value);
    };

    if (data.contains("last_name"))
        add_param("last_name", data["last_name"]);
    if (data.contains("first_name"))
        add_param("first_name", data["first_name"]);
    if (data.contains("patronymic"))
        add_param("patronymic", data["patronymic"]);

    if (data.contains("new_password") || data.contains("new_password_repeat")) {
        if (!data.contains("new_password") || !data.contains("new_password_repeat")) {
            response["success"] = false;
            response["error"] = "Both new_password and new_password_repeat must be provided";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string np1 = data["new_password"];
        std::string np2 = data["new_password_repeat"];
        if (np1 != np2) {
            response["success"] = false;
            response["error"] = "New passwords do not match";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }

        std::string new_salt = db_handler.generate_salt(16);
        std::string new_hash = db_handler.hash_password(np1, new_salt);
        add_param("hashed_password", new_hash);
        add_param("salt", new_salt);
    }

    if (clauses.empty()) {
        response["success"] = false;
        response["error"] = "No fields to update";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::ostringstream ss;
    ss << "UPDATE users SET ";
    for (size_t i = 0; i < clauses.size(); ++i) {
        if (i) ss << ", ";
        ss << clauses[i];
    }
    ss << " WHERE id = $" << (param_values.size() + 1);

    param_values.push_back(user_id_str);
    std::vector<const char *> param_ptrs;
    for (const auto &val : param_values)
        param_ptrs.push_back(val.c_str());

    PGresult *upd = PQexecParams(
        conn,
        ss.str().c_str(),
        static_cast<int>(param_ptrs.size()), nullptr,
        param_ptrs.data(), nullptr, nullptr, 0
    );

    if (!upd || PQresultStatus(upd) != PGRES_COMMAND_OK) {
        std::string err = PQerrorMessage(conn);
        if (upd) PQclear(upd);
        response["success"] = false;
        response["error"] = "Database error: " + err;
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    PQclear(upd);
    response["success"] = true;
    response["message"] = "Profile updated successfully";
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
