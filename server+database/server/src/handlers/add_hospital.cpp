#include "../../include/handlers/add_hospital.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <regex>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void add_hospital(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    nlohmann::json response;

    // Проверка на наличие обязательных полей
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
    int admin_id = data["admin_id"];

    // Проверка, если такой адрес уже существует в базе
    const char* params_exist[5] = { region.c_str(), settlement_type.c_str(), settlement_name.c_str(), street.c_str(), house.c_str() };
    PGresult *res_exist = PQexecParams(db_handler.get_connection(),
        "SELECT 1 FROM hospitals WHERE region = $1 AND settlement_type = $2 AND settlement_name = $3 AND street = $4 AND house = $5",
        5, NULL, params_exist, NULL, NULL, 0);
    if (PQresultStatus(res_exist) == PGRES_TUPLES_OK && PQntuples(res_exist) > 0) {
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

    // Вставка нового госпиталя в базу
    std::string admin_id_str = std::to_string(admin_id);
    const char* params_ins[7] = { region.c_str(), settlement_type.c_str(), settlement_name.c_str(), street.c_str(), house.c_str(), full_name.c_str(), admin_id_str.c_str() };
    PGresult *res_ins = PQexecParams(db_handler.get_connection(),
        "INSERT INTO hospitals (region, settlement_type, settlement_name, street, house, full_name, administrator_id) VALUES ($1, $2, $3, $4, $5, $6, $7)",
        7, NULL, params_ins, NULL, NULL, 0);

    // Проверка, если госпиталь был успешно добавлен
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
        res.result(http::status::internal_server_error);  // 500 Internal Server Error
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
