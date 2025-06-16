#include "../../include/handlers/get_admin_hospital.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_admin_hospital(int admin_id, http::response<http::string_body> &res, database_handler &db_handler) {
    PGconn* conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database connection failed"})";
        return;
    }

    std::string query = "SELECT h.hospital_id, h.full_name, h.region, h.settlement_type, h.settlement_name, "
                        "h.street, h.house, h.time_open, h.junior_admin_phone "
                        "FROM hospitals h "
                        "WHERE h.administrator_id = $1";

    const char* param_values[1];
    std::string admin_id_str = std::to_string(admin_id);
    param_values[0] = admin_id_str.c_str();

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
        res.body() = R"({"success": false, "error": "Hospital not found for this administrator"})";
        return;
    }    json response;
    json hospital;
    hospital["hospital_id"] = std::stoi(PQgetvalue(result, 0, 0));
    hospital["full_name"] = PQgetvalue(result, 0, 1);
    hospital["region"] = PQgetvalue(result, 0, 2);
    hospital["settlement_type"] = PQgetvalue(result, 0, 3);
    hospital["settlement_name"] = PQgetvalue(result, 0, 4);
    hospital["street"] = PQgetvalue(result, 0, 5);
    hospital["house"] = PQgetvalue(result, 0, 6);

    if (!PQgetisnull(result, 0, 7)) {
        hospital["time_open"] = PQgetvalue(result, 0, 7);
    } else {
        hospital["time_open"] = "";
    }

    if (!PQgetisnull(result, 0, 8)) {
        hospital["junior_admin_phone"] = PQgetvalue(result, 0, 8);
    } else {
        hospital["junior_admin_phone"] = "";
    }

    response["success"] = true;
    response["hospital"] = hospital;

    PQclear(result);

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
