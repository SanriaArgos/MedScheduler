#include "add_hospital.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <regex>

extern database_handler* global_db;

bool add_hospital(const nlohmann::json &data) {
    if (!data.contains("region") || !data.contains("settlement_type") ||
        !data.contains("settlement_name") || !data.contains("street") ||
        !data.contains("house") || !data.contains("full_name") ||
        !data.contains("admin_id")) {
        std::cerr << "Error: Missing required fields for adding hospital\n";
        return false;
    }
    std::string region = data["region"];
    std::string settlement_type = data["settlement_type"];
    std::string settlement_name = data["settlement_name"];
    std::string street = data["street"];
    std::string house = data["house"];
    std::string full_name = data["full_name"];
    int admin_id = data["admin_id"];

    // Все проверки выполняются с использованием параметризованных запросов
    const char* params_exist[5] = { region.c_str(), settlement_type.c_str(), settlement_name.c_str(), street.c_str(), house.c_str() };
    PGresult *res_exist = PQexecParams(global_db->get_connection(),
        "SELECT 1 FROM hospitals WHERE region = $1 AND settlement_type = $2 AND settlement_name = $3 AND street = $4 AND house = $5",
        5, NULL, params_exist, NULL, NULL, 0);
    if (PQresultStatus(res_exist) == PGRES_TUPLES_OK && PQntuples(res_exist) > 0) {
        std::cerr << "Error: Address already in use\n";
        PQclear(res_exist);
        return false;
    }
    PQclear(res_exist);
    
    std::string admin_id_str = std::to_string(admin_id);
    const char* params_ins[7] = { region.c_str(), settlement_type.c_str(), settlement_name.c_str(), street.c_str(), house.c_str(), full_name.c_str(), admin_id_str.c_str() };
    PGresult *res_ins = PQexecParams(global_db->get_connection(),
        "INSERT INTO hospitals (region, settlement_type, settlement_name, street, house, full_name, administrator_id) VALUES ($1, $2, $3, $4, $5, $6, $7)",
        7, NULL, params_ins, NULL, NULL, 0);
    if (PQresultStatus(res_ins) == PGRES_COMMAND_OK) {
        std::cerr << "Hospital added\n";
        PQclear(res_ins);
        return true;
    } else {
        std::cerr << "Error adding hospital\n";
        PQclear(res_ins);
        return false;
    }
}
