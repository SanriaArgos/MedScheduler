#include "../../include/handlers/add_hospital.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler *global_db;

void add_hospital(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    nlohmann::json response;

    if (!data.contains("region") || !data.contains("settlement_type") ||
        !data.contains("settlement_name") || !data.contains("street") ||
        !data.contains("house") || !data.contains("full_name") ||
        !data.contains("admin_id")) {
        std::cerr << "Error: Missing required fields for adding hospital\n";
        response["success"] = false;
        response["error"] = "Missing required fields";

        res.result(http::status::bad_request);  // 400 Bad Request
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string region = data["region"];
    std::string settlement_type = data["settlement_type"];
    std::string settlement_name = data["settlement_name"];
    std::string street = data["street"];
    std::string house = data["house"];
    std::string full_name = data["full_name"];
    std::string admin_id = data["admin_id"];

    const char *params_exist[5] = {
        region.c_str(), settlement_type.c_str(), settlement_name.c_str(),
        street.c_str(), house.c_str()};
    PGresult *res_exist = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE region = $1 AND settlement_type = $2 "
        "AND settlement_name = $3 AND street = $4 AND house = $5",
        5, NULL, params_exist, NULL, NULL, 0
    );
    if (PQresultStatus(res_exist) == PGRES_TUPLES_OK &&
        PQntuples(res_exist) > 0) {
        std::cerr << "Error: Address already in use\n";
        response["success"] = false;
        response["error"] = "Address already in use";

        PQclear(res_exist);
        res.result(http::status::conflict);  // 409 Conflict
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(res_exist);

    const char *auth_params[1] = {admin_id.c_str()};
    PGresult *res_auth = PQexecParams(
        db_handler.get_connection(),
        "SELECT user_type FROM users WHERE id = $1", 1, nullptr, auth_params,
        nullptr, nullptr, 0
    );
    if (!res_auth || PQresultStatus(res_auth) != PGRES_TUPLES_OK ||
        PQntuples(res_auth) == 0) {
        if (res_auth) {
            PQclear(res_auth);
        }
        std::cerr << "Error: Admin user not found\n";
        response["success"] = false;
        response["error"] = "Admin user not found";

        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    std::string user_type = PQgetvalue(res_auth, 0, 0);
    PQclear(res_auth);

    if (user_type != "junior administrator") {
        std::cerr << "Error: User is not junior administrator\n";
        response["success"] = false;
        response["error"] = "User is not junior administrator";

        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    const char *check_params[1] = {admin_id.c_str()};
    PGresult *res_check_admin = PQexecParams(
        db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE administrator_id = $1", 1, nullptr,
        check_params, nullptr, nullptr, 0
    );
    if (PQresultStatus(res_check_admin) == PGRES_TUPLES_OK &&
        PQntuples(res_check_admin) > 0) {
        std::cerr
            << "Error: Junior administrator already assigned to a hospital\n";
        response["success"] = false;
        response["error"] =
            "Junior administrator already assigned to a hospital";

        PQclear(res_check_admin);
        res.result(http::status::conflict);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(res_check_admin);

    std::string admin_id_str = admin_id;
    const char *params_ins[7] = {
        region.c_str(),      settlement_type.c_str(), settlement_name.c_str(),
        street.c_str(),      house.c_str(),           full_name.c_str(),
        admin_id_str.c_str()};
    PGresult *res_ins = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO hospitals (region, settlement_type, settlement_name, "
        "street, house, full_name, administrator_id) VALUES ($1, $2, $3, $4, "
        "$5, $6, $7)",
        7, NULL, params_ins, NULL, NULL, 0
    );

    if (PQresultStatus(res_ins) == PGRES_COMMAND_OK) {
        std::cerr << "Hospital added\n";
        response["success"] = true;
        response["message"] = "Hospital successfully added";

        PQclear(res_ins);
        res.result(http::status::ok);  // 200 OK
    } else {
        std::cerr << "Error adding hospital\n";
        response["success"] = false;
        response["error"] = "Error adding hospital";

        PQclear(res_ins);
        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
