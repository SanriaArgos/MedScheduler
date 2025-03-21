#include "add_hospital.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <regex>

extern database_handler* global_db;

bool add_hospital(const std::string &region,
                  const std::string &settlement_type,
                  const std::string &settlement_name,
                  const std::string &street,
                  const std::string &house,
                  const std::string &full_name,
                  int admin_id) {
    std::regex latin_regex("^[A-Za-z\\-]+$");
    if (!std::regex_match(region, latin_regex)) {
        std::cerr << "Error: Region must contain only Latin letters and '-'\n";
        return false;
    }
    if (!std::regex_match(settlement_type, latin_regex)) {
        std::cerr << "Error: Settlement_Type must contain only Latin letters and '-'\n";
        return false;
    }
    if (!std::regex_match(settlement_name, latin_regex)) {
        std::cerr << "Error: Settlement_Name must contain only Latin letters and '-'\n";
        return false;
    }
    if (!std::regex_match(street, latin_regex)) {
        std::cerr << "Error: Street must contain only Latin letters and '-'\n";
        return false;
    }
    if (!std::all_of(house.begin(), house.end(), ::isdigit)) {
        std::cerr << "Error: House must contain only digits\n";
        return false;
    }
    // Проверяем наличие адреса с помощью параметризованного запроса.
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
    
    // Проверяем администратора с помощью параметризованного запроса.
    std::string admin_id_str = std::to_string(admin_id);
    const char* params_admin[1] = { admin_id_str.c_str() };
    PGresult *res_admin = PQexecParams(global_db->get_connection(),
        "SELECT user_type FROM users WHERE id = $1",
        1, NULL, params_admin, NULL, NULL, 0);
    if (!(PQresultStatus(res_admin) == PGRES_TUPLES_OK && PQntuples(res_admin) > 0)) {
        std::cerr << "Error: Administrator ID not found\n";
        PQclear(res_admin);
        return false;
    }
    std::string user_type = PQgetvalue(res_admin, 0, 0);
    PQclear(res_admin);
    if (user_type != "junior administrator") {
        std::cerr << "Error: ID does not belong to a junior administrator\n";
        return false;
    }
    
    // Вставляем данные больницы с помощью параметризованного запроса.
    const char* params_ins[7] = { region.c_str(), settlement_type.c_str(), settlement_name.c_str(),
                                  street.c_str(), house.c_str(), full_name.c_str(), admin_id_str.c_str() };
    PGresult *res_ins = PQexecParams(global_db->get_connection(),
        "INSERT INTO hospitals (region, settlement_type, settlement_name, street, house, full_name, administrator_id) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7)",
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
