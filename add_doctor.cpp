#include "add_doctor.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <regex>
#include <cstdlib>

void add_doctor(database_handler &db, int junior_admin_id) {
    std::cout << "\n=== Add Doctor ===\n";
    
    std::string last_name = get_validated_name("Last Name", true);
    std::string first_name = get_validated_name("First Name", true);
    std::string patronymic = get_input("Patronymic (optional): ");
    if (!patronymic.empty()) {
        while (!is_latin_or_dash(patronymic)) {
            std::cout << "Error: Use only Latin letters and '-'\n";
            patronymic = get_input("Patronymic (optional): ");
        }
    }
    
    std::string phone = get_validated_phone();
    while (db.user_exists(phone)) {
        std::cout << "Error: Phone already registered\n";
        phone = get_validated_phone();
    }
    
    const std::string default_password = "987654321";
    if (!db.register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cout << "Error: Could not add doctor to Users table\n";
        return;
    }
    
    std::string query = "UPDATE users SET user_type = 'doctor' WHERE phone = '" + phone + "'";
    PGresult *res = PQexec(db.get_connection(), query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error: Could not update user type: " 
                  << PQerrorMessage(db.get_connection()) << "\n";
    }
    PQclear(res);
    std::cout << "Doctor added to Users table\n";
    
    query = "SELECT id FROM users WHERE phone = '" + phone + "'";
    res = PQexec(db.get_connection(), query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        std::cout << "Error: Could not retrieve doctor ID\n";
        if (res) PQclear(res);
        return;
    }
    int user_id = static_cast<int>(std::stoi(PQgetvalue(res, 0, 0)));
    PQclear(res);
    
    std::string education = get_validated_input("Education (only Latin letters and '-')", true);
    std::regex valid_regex("^[A-Za-z\\-]+$");
    while (!std::regex_match(education, valid_regex)) {
        std::cout << "Error: Education must contain only Latin letters and '-'\n";
        education = get_validated_input("Education (only Latin letters and '-')", true);
    }
    
    std::string specialty = get_validated_input("Specialty (only Latin letters and '-')", true);
    while (!std::regex_match(specialty, valid_regex)) {
        std::cout << "Error: Specialty must contain only Latin letters and '-'\n";
        specialty = get_validated_input("Specialty (only Latin letters and '-')", true);
    }
    
    std::string exp_str = get_validated_input("Experience (only digits)", true);
    while (!std::all_of(exp_str.begin(), exp_str.end(), ::isdigit)) {
        std::cout << "Error: Experience must contain only digits\n";
        exp_str = get_validated_input("Experience (only digits)", true);
    }
    int experience = static_cast<int>(std::stoi(exp_str));
    
    std::stringstream ss;
    ss << "INSERT INTO doctors (user_id, education, specialty, experience, hospital_ids) VALUES ("
       << user_id << ", '" << education << "', '" << specialty << "', " << experience << ", '{}')";
    res = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        std::cout << "Doctor data successfully added to Doctors table\n";
    } else {
        std::cout << "Error: Could not add data to Doctors table\n";
    }
    PQclear(res);
}
