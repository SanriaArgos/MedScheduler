#include "../../include/handlers/auth_handler.hpp"
#include <libpq-fe.h>
#include <iostream>
#include "../../include/database.hpp"

std::string login_user(
    database_handler &db,
    const std::string &phone,
    const std::string &password
) {
    PGconn *conn = db.get_connection();
    const char *paramValues[1] = {phone.c_str()};
    PGresult *res = PQexecParams(
        conn,
        "SELECT id, user_type, hashed_password, salt FROM users WHERE phone = "
        "$1",
        1, NULL, paramValues, NULL, NULL, 0
    );
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (res) {
            PQclear(res);
        }
        return "";
    }

    std::string user_id = PQgetvalue(res, 0, 0);
    std::string user_type = PQgetvalue(res, 0, 1);
    std::string db_hash = PQgetvalue(res, 0, 2);
    std::string db_salt = PQgetvalue(res, 0, 3);
    PQclear(res);

    std::string attempt_hash = db.hash_password(password, db_salt);
    if (attempt_hash != db_hash) {
        return "";
    }

    {
        std::string upd_sql =
            "UPDATE users SET last_login = NOW() WHERE id = " + user_id;
        PGresult *r_up = PQexec(conn, upd_sql.c_str());
        if (r_up) PQclear(r_up);
    }

    if (user_type == "patient") {
        return "patient:" + user_id;
    }
    if (user_type == "senior administrator") {
        return "senior:" + user_id;
    }
    if (user_type == "junior administrator") {
        return "junior:" + user_id;
    }
    if (user_type == "doctor") {
        std::string query2 =
            "SELECT doctor_id FROM doctors WHERE user_id = " + user_id;
        PGresult *res2 = PQexec(conn, query2.c_str());
        if (!res2 || PQresultStatus(res2) != PGRES_TUPLES_OK ||
            PQntuples(res2) == 0) {
            if (res2) {
                PQclear(res2);
            }
            return "";
        }
        std::string doctor_id = PQgetvalue(res2, 0, 0);
        PQclear(res2);
        return "doctor:" + doctor_id;
    }

    return "success";
}

bool register_user(
    database_handler &db,
    const std::string &last_name,
    const std::string &first_name,
    const std::string &patronymic,
    const std::string &phone,
    const std::string &password
) {
    std::string salt = db.generate_salt(16);
    std::string hashed_pass = db.hash_password(password, salt);
    std::string query =
        "INSERT INTO users (last_name, first_name, patronymic, phone, "
        "hashed_password, salt, user_type) VALUES ('" +
        db.prepare_query(last_name) + "', '" + db.prepare_query(first_name) +
        "', '" + db.prepare_query(patronymic) + "', '" +
        db.prepare_query(phone) + "', '" + db.prepare_query(hashed_pass) +
        "', '" + db.prepare_query(salt) + "', 'patient')";

    PGresult *res = PQexec(db.get_connection(), query.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    if (!success) {
        std::cerr << "Ошибка регистрации: "
                  << PQerrorMessage(db.get_connection()) << "\n";
    }
    PQclear(res);
    return success;
}

std::string
get_user_type_by_phone(database_handler &db, const std::string &phone) {
    const char *params[1] = {phone.c_str()};
    PGresult *res = PQexecParams(
        db.get_connection(), "SELECT user_type FROM users WHERE phone = $1", 1,
        nullptr, params, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "DB error (get_user_type): "
                      << PQerrorMessage(db.get_connection());
        }
        PQclear(res);
        return "";
    }
    std::string user_type = PQgetvalue(res, 0, 0);
    PQclear(res);
    return user_type;
}

int get_user_id_by_phone(database_handler &db, const std::string &phone) {
    const char *params[1] = {phone.c_str()};
    PGresult *res = PQexecParams(
        db.get_connection(), "SELECT id FROM users WHERE phone = $1", 1,
        nullptr, params, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "DB error (get_user_id): "
                      << PQerrorMessage(db.get_connection());
        }
        PQclear(res);
        return -1;
    }
    int id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return id;
}

bool user_exists(database_handler &db, const std::string &phone) {
    std::string query =
        "SELECT 1 FROM users WHERE phone = '" + db.prepare_query(phone) + "'";
    PGresult *res = PQexec(db.get_connection(), query.c_str());
    bool exists =
        (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}