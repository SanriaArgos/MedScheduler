#include "add_hospital.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <regex>
#include <locale>
#include <codecvt>

// Функция-обёртка для проверки поля с ограничением: только латинские буквы и '-'
static std::string get_validated_latin_field(const std::string &prompt) {
    while (true) {
        std::string input = get_validated_input(prompt, true);
        if (!is_latin_or_dash(input)) {
            std::cout << "Error: Field must contain only Latin letters and '-'" << std::endl;
            continue;
        }
        return input;
    }
}

static std::string get_validated_numeric_field(const std::string &prompt) {
    while (true) {
        std::string input = get_validated_input(prompt, true);
        if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cout << "Error: Field must contain only digits" << std::endl;
            continue;
        }
        return input;
    }
}

void add_hospital(database_handler &db) {
    std::cout << "\n=== Add New Hospital ===\n";
    std::string region = get_validated_latin_field("Region");
    std::string settlement_type = get_validated_latin_field("Settlement_Type");
    std::string settlement_name = get_validated_latin_field("Settlement_Name");
    std::string street = get_validated_latin_field("Street");
    std::string house = get_validated_numeric_field("House");
    std::string full_name = get_validated_input("Full_Name", true);
    
    std::stringstream ss_exist;
    ss_exist << "SELECT 1 FROM hospitals WHERE region = '" << region << "' AND settlement_type = '"
             << settlement_type << "' AND settlement_name = '" << settlement_name
             << "' AND street = '" << street << "' AND house = '" << house << "'";
    PGresult *res_exist = PQexec(db.get_connection(), ss_exist.str().c_str());
    if (PQresultStatus(res_exist) == PGRES_TUPLES_OK && PQntuples(res_exist) > 0) {
        std::cout << "Error: Address already in use" << std::endl;
        PQclear(res_exist);
        return;
    }
    PQclear(res_exist);
    
    std::string admin_id_str = get_validated_numeric_field("ID of Junior Administrator");
    int admin_id = static_cast<int>(std::stoi(admin_id_str));
    std::stringstream ss_check_admin;
    ss_check_admin << "SELECT user_type FROM users WHERE id = " << admin_id;
    PGresult *res_admin = PQexec(db.get_connection(), ss_check_admin.str().c_str());
    if (!(PQresultStatus(res_admin) == PGRES_TUPLES_OK && PQntuples(res_admin) > 0)) {
        std::cout << "Error: Administrator ID not found" << std::endl;
        PQclear(res_admin);
        return;
    }
    std::string user_type = PQgetvalue(res_admin, 0, 0);
    PQclear(res_admin);
    if (user_type != "junior administrator") {
        std::cout << "Error: ID does not belong to a junior administrator" << std::endl;
        return;
    }
    
    std::stringstream ss;
    ss << "INSERT INTO hospitals (region, settlement_type, settlement_name, street, house, full_name, administrator_id) VALUES ('"
       << region << "', '" << settlement_type << "', '" << settlement_name << "', '" << street << "', '"
       << house << "', '" << full_name << "', " << admin_id << ")";
    PGresult *res_ins = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res_ins) == PGRES_COMMAND_OK) {
        std::cout << "Hospital added" << std::endl;
    } else {
        std::cout << "Error adding hospital" << std::endl;
    }
    PQclear(res_ins);
}
