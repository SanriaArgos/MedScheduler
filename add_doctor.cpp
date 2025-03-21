#include "add_doctor.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <regex>
#include <cstdlib>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern database_handler* global_db;

bool add_doctor(const json &data) {
    if (!data.contains("last_name") || !data.contains("first_name") ||
        !data.contains("phone") || !data.contains("education") ||
        !data.contains("specialty") || !data.contains("experience")) {
        std::cerr << "Error: Missing required fields for add_doctor\n";
        return false;
    }
    std::string last_name = data["last_name"];
    std::string first_name = data["first_name"];
    std::string patronymic = data.value("patronymic", "");
    std::string phone = data["phone"];
    std::string education = data["education"];
    std::string specialty = data["specialty"];
    int experience = data["experience"];
    
    if (global_db->user_exists(phone)) {
        std::cerr << "Error: Phone already registered\n";
        return false;
    }
    
    const std::string default_password = "987654321";
    if (!global_db->register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cerr << "Error: Could not add doctor to Users table\n";
        return false;
    }
    
    const char* params_update[1] = { phone.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "UPDATE users SET user_type = 'doctor' WHERE phone = $1",
        1, NULL, params_update, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error: Could not update user type: " 
                  << PQerrorMessage(global_db->get_connection()) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    
    res = PQexecParams(global_db->get_connection(),
        "SELECT id FROM users WHERE phone = $1",
        1, NULL, params_update, NULL, NULL, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        std::cerr << "Error: Could not retrieve doctor ID\n";
        if (res) PQclear(res);
        return false;
    }
    int user_id = static_cast<int>(std::stoi(PQgetvalue(res, 0, 0)));
    PQclear(res);
    
    std::regex valid_regex("^[A-Za-z\\-]+$");
    if (!std::regex_match(education, valid_regex)) {
        std::cerr << "Error: Education must contain only Latin letters and '-'\n";
        return false;
    }
    if (!std::regex_match(specialty, valid_regex)) {
        std::cerr << "Error: Specialty must contain only Latin letters and '-'\n";
        return false;
    }
    
    std::string user_id_str = std::to_string(user_id);
    std::string experience_str = std::to_string(experience);
    const char* params_ins[5] = { user_id_str.c_str(), education.c_str(), specialty.c_str(), experience_str.c_str(), "{}" };
    res = PQexecParams(global_db->get_connection(),
        "INSERT INTO doctors (user_id, education, specialty, experience, hospital_ids) VALUES ($1, $2, $3, $4, $5)",
        5, NULL, params_ins, NULL, NULL, 0);
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        std::cerr << "Doctor data successfully added to Doctors table\n";
        PQclear(res);
        return true;
    } else {
        std::cerr << "Error: Could not add data to Doctors table\n";
        PQclear(res);
        return false;
    }
}
