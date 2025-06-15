#include "../../include/handlers/edit_doctor_profile.hpp"
#include <libpq-fe.h>
#include <vector>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

static bool
phone_is_unique(int user_id, const std::string &phone, PGconn *conn) {
    const char *params[2] = {phone.c_str(), std::to_string(user_id).c_str()};
    PGresult *r = PQexecParams(
        conn, "SELECT 1 FROM users WHERE phone = $1 AND id <> $2 LIMIT 1", 2,
        nullptr, params, nullptr, nullptr, 0
    );
    bool ok = (r && PQresultStatus(r) == PGRES_TUPLES_OK && PQntuples(r) == 0);
    if (r) PQclear(r);
    return ok;
}

void edit_doctor_profile(
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

    int doctor_id = data["user_id"];
    std::string query = "SELECT user_id FROM doctors WHERE doctor_id = $1";
    const char* params[1] = { std::to_string(doctor_id).c_str() };
    PGresult* ress = PQexecParams(db_handler.get_connection(), query.c_str(), 1, nullptr, params, nullptr, nullptr, 0);

    if (!ress || PQresultStatus(ress) != PGRES_TUPLES_OK || PQntuples(ress) == 0) {
        if (ress) PQclear(ress);
        return; 
    }
    std::string user_id_str = PQgetvalue(ress, 0, 0);
    PQclear(ress);
    int user_id=std::stoi(user_id_str);
    std::string current_password = data["current_password"];

    // Проверяем user_type, hashed_password и salt
    const char *p1[1] = { std::to_string(user_id).c_str() };
    PGresult *creds = PQexecParams(
        db_handler.get_connection(),
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
    std::string db_salt = PQgetvalue(creds, 0, 2);
    PQclear(creds);

    if (user_type != "doctor") {
        response["success"] = false;
        response["error"] = "User is not a doctor";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Проверка пароля
    if (db_handler.hash_password(current_password, db_salt) != db_hashed) {
        response["success"] = false;
        response["error"] = "Invalid current_password";
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::vector<std::string> clauses;
    std::vector<const char *> params;
    int param_index = 1;
    std::vector<std::string> string_storage; // для хранения строк, чтобы указатели не ломались

    auto add_text_param = [&](const std::string &field, const std::string &value) {
        clauses.push_back(field + " = $" + std::to_string(param_index));
        string_storage.push_back(value);
        params.push_back(string_storage.back().c_str());
        param_index++;
    };

    if (data.contains("last_name"))  add_text_param("last_name",  data["last_name"]);
    if (data.contains("first_name")) add_text_param("first_name", data["first_name"]);
    if (data.contains("patronymic")) add_text_param("patronymic", data["patronymic"]);
    if (data.contains("phone")) {
        std::string phone = data["phone"];
        if (!phone_is_unique(user_id, phone, db_handler.get_connection())) {
            response["success"] = false;
            response["error"] = "Phone already in use";
            res.result(http::status::conflict);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        add_text_param("phone", phone);
    }

    // Обновление пароля
    if (data.contains("new_password") || data.contains("new_password_repeat")) {
        if (!data.contains("new_password") ||
            !data.contains("new_password_repeat")) {
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

    std::ostringstream ss;
    ss << "UPDATE users SET ";
    for (size_t i = 0; i < clauses.size(); ++i) {
        if (i) ss << ", ";
        ss << clauses[i];
    }
    ss << " WHERE id = $" << param_index;
    string_storage.push_back(std::to_string(user_id));
    params.push_back(string_storage.back().c_str());

    PGresult *upd = PQexecParams(
        db_handler.get_connection(),
        ss.str().c_str(),
        (int)params.size(), nullptr, params.data(), nullptr, nullptr, 0
    );

    if (!upd || PQresultStatus(upd) != PGRES_COMMAND_OK) {
        std::string err = upd ? PQerrorMessage(db_handler.get_connection()) : "Unknown error";
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
