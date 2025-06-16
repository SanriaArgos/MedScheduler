#include "../../include/handlers/get_junior_admin_profile.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <sstream>

void get_junior_admin_profile(int user_id, http::response<http::string_body> &res, database_handler &db_handler) {
    PGconn* conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database connection failed"})";
        return;
    }

    std::string query = "SELECT u.id, u.last_name, u.first_name, u.patronymic, u.phone, u.user_type "
                        "FROM users u "
                        "WHERE u.id = $1 AND u.user_type = 'junior_admin'";

    const char* param_values[1];
    std::string user_id_str = std::to_string(user_id);
    param_values[0] = user_id_str.c_str();

    PGresult* result = PQexecParams(conn, query.c_str(), 1, nullptr, param_values, nullptr, nullptr, 0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::string err_msg = PQerrorMessage(conn);
        PQclear(result);
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database query failed: )" + err_msg + R"("})";
        return;
    }

    if (PQntuples(result) == 0) {
        PQclear(result);
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Junior admin not found"})";
        return;
    }

    json profile;
    profile["success"] = true;
    profile["user_id"] = user_id;
    profile["last_name"] = PQgetvalue(result, 0, 1);
    profile["first_name"] = PQgetvalue(result, 0, 2);

    if (!PQgetisnull(result, 0, 3)) {
        profile["patronymic"] = PQgetvalue(result, 0, 3);
    } else {
        profile["patronymic"] = "";
    }

    profile["phone"] = PQgetvalue(result, 0, 4);
    profile["user_type"] = PQgetvalue(result, 0, 5);

    PQclear(result);

    // Теперь получаем информацию о больнице, к которой привязан администратор
    query = "SELECT h.hospital_id, h.full_name, h.region, h.settlement_type, h.settlement_name, h.street, h.house "
            "FROM hospitals h "
            "WHERE h.administrator_id = $1";

    PGresult* hospital_result = PQexecParams(conn, query.c_str(), 1, nullptr, param_values, nullptr, nullptr, 0);

    if (PQresultStatus(hospital_result) == PGRES_TUPLES_OK && PQntuples(hospital_result) > 0) {
        profile["hospital_id"] = std::stoi(PQgetvalue(hospital_result, 0, 0));
        profile["hospital_name"] = PQgetvalue(hospital_result, 0, 1);
        profile["hospital_region"] = PQgetvalue(hospital_result, 0, 2);
        profile["hospital_settlement_type"] = PQgetvalue(hospital_result, 0, 3);
        profile["hospital_settlement_name"] = PQgetvalue(hospital_result, 0, 4);
        profile["hospital_street"] = PQgetvalue(hospital_result, 0, 5);
        profile["hospital_house"] = PQgetvalue(hospital_result, 0, 6);
    }

    PQclear(hospital_result);

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = profile.dump();
}
