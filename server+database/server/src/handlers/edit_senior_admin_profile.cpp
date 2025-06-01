#include "../../include/handlers/edit_senior_admin_profile.hpp"
#include <libpq-fe.h>
#include <openssl/sha.h>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Проверка уникальности телефона (скопировано из edit_patient_profile.cpp)
static bool phone_is_unique(int user_id, const std::string &phone, PGconn *conn) {
    const char *params[2] = { phone.c_str(), std::to_string(user_id).c_str() };
    PGresult *r = PQexecParams(
        conn,
        "SELECT 1 FROM users WHERE phone = $1 AND id <> $2 LIMIT 1",
        2, nullptr, params, nullptr, nullptr, 0
    );
    bool exists = (r && PQntuples(r) > 0 && PQresultStatus(r) == PGRES_TUPLES_OK);
    if (r) PQclear(r);
    return !exists;
}

void edit_senior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1) Проверяем, что есть user_id и current_password
    if (!data.contains("user_id") || !data.contains("current_password")) {
        response["success"] = false;
        response["error"]   = "Missing 'user_id' or 'current_password'";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int user_id = data["user_id"].get<int>();
    std::string current_password = data["current_password"];

    // 2) Проверим, что user_type = "senior administrator"
    {
        const char *uid_param[1] = { std::to_string(user_id).c_str() };
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_type FROM users WHERE id = $1",
            1, nullptr, uid_param, nullptr, nullptr, 0
        );
        if (!r || PQresultStatus(r) != PGRES_TUPLES_OK || PQntuples(r) == 0) {
            if (r) PQclear(r);
            response["success"] = false;
            response["error"]   = "User not found";
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        std::string ut = PQgetvalue(r, 0, 0);
        PQclear(r);
        if (ut != "senior administrator") {
            response["success"] = false;
            response["error"]   = "Not a senior administrator";
            res.result(http::status::forbidden);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    // 3) Проверка текущего пароля
    std::string db_hashed, db_salt;
    {
        const char *uid_param[1] = { std::to_string(user_id).c_str() };
        PGresult *r = PQexecParams(
            db_handler.get_connection(),
            "SELECT hashed_password, salt FROM users WHERE id = $1",
            1, nullptr, uid_param, nullptr, nullptr, 0
        );
        if (!r || PQresultStatus(r) != PGRES_TUPLES_OK || PQntuples(r) == 0) {
            if (r) PQclear(r);
            response["success"] = false;
            response["error"]   = "User not found";
            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        db_hashed = PQgetvalue(r, 0, 0);
        db_salt   = PQgetvalue(r, 0, 1);
        PQclear(r);
    }
    std::string to_hash = current_password + db_salt;
    unsigned char hbuf[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(to_hash.c_str()), to_hash.size(), hbuf);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hbuf[i];
    }
    if (ss.str() != db_hashed) {
        response["success"] = false;
        response["error"]   = "Current password is incorrect";
        res.result(http::status::unauthorized);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4) Составляем список полей для UPDATE (phone разрешено)
    std::vector<std::string> clauses;
    std::vector<const char*>  params;
    int idx = 1;

    if (data.contains("last_name")) {
        clauses.push_back("last_name = $" + std::to_string(idx) + "::text");
        params.push_back(data["last_name"].get<std::string>().c_str());
        idx++;
    }
    if (data.contains("first_name")) {
        clauses.push_back("first_name = $" + std::to_string(idx) + "::text");
        params.push_back(data["first_name"].get<std::string>().c_str());
        idx++;
    }
    if (data.contains("patronymic")) {
        clauses.push_back("patronymic = $" + std::to_string(idx) + "::text");
        params.push_back(data["patronymic"].get<std::string>().c_str());
        idx++;
    }
    if (data.contains("phone")) {
        std::string new_phone = data["phone"].get<std::string>();
        if (!phone_is_unique(user_id, new_phone, db_handler.get_connection())) {
            response["success"] = false;
            response["error"]   = "Phone already in use";
            res.result(http::status::conflict);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        clauses.push_back("phone = $" + std::to_string(idx) + "::text");
        params.push_back(new_phone.c_str());
        idx++;
    }

    bool change_password = false;
    if (data.contains("new_password") || data.contains("new_password_repeat")) {
        if (!data.contains("new_password") || !data.contains("new_password_repeat")) {
            response["success"] = false;
            response["error"]   = "Both 'new_password' and 'new_password_repeat' must be present";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        std::string np1 = data["new_password"].get<std::string>();
        std::string np2 = data["new_password_repeat"].get<std::string>();
        if (np1 != np2) {
            response["success"] = false;
            response["error"]   = "New passwords do not match";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        std::string new_salt = db_handler.generate_salt(16);
        std::string tohash = np1 + new_salt;
        unsigned char h2[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char *>(tohash.c_str()), tohash.size(), h2);
        std::stringstream ss2;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            ss2 << std::hex << std::setw(2) << std::setfill('0') << (int)h2[i];
        }
        std::string new_hash = ss2.str();

        clauses.push_back("hashed_password = $" + std::to_string(idx) + "::text");
        params.push_back(new_hash.c_str());
        idx++;

        clauses.push_back("salt = $" + std::to_string(idx) + "::text");
        params.push_back(new_salt.c_str());
        idx++;

        change_password = true;
    }

    if (clauses.empty()) {
        response["success"] = false;
        response["error"]   = "No fields to update";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 5) UPDATE users
    std::ostringstream sql;
    sql << "UPDATE users SET ";
    for (size_t i = 0; i < clauses.size(); ++i) {
        if (i > 0) sql << ", ";
        sql << clauses[i];
    }
    sql << " WHERE id = $" << idx << "::int";
    params.push_back(std::to_string(user_id).c_str());

    PGresult *ures2 = PQexecParams(
        db_handler.get_connection(),
        sql.str().c_str(),
        (int)params.size(),
        nullptr,
        params.data(),
        nullptr,
        nullptr,
        0
    );
    if (!ures2 || PQresultStatus(ures2) != PGRES_COMMAND_OK) {
        if (ures2) PQclear(ures2);
        response["success"] = false;
        response["error"]   = "Failed to update user data";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(ures2);

    response["success"] = true;
    response["message"] = "Senior admin profile updated successfully";
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
