#include "../../include/handlers/registration.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

extern database_handler* global_db;  // Объявляем глобальный указатель, определённый в main.cpp

bool registration(const nlohmann::json &data) {
    if (!data.contains("last_name") || !data.contains("first_name") ||
        !data.contains("phone") || !data.contains("password")) {
        std::cerr << "Registration error: missing required fields\n";
        return false;
    }
    std::string last_name = data["last_name"];
    std::string first_name = data["first_name"];
    std::string patronymic = data.value("patronymic", "");
    std::string phone = data["phone"];
    std::string password = data["password"];
    
    if (global_db->register_user(last_name, first_name, patronymic, phone, password)) {
        std::cerr << "Registration successful\n";
        return true;
    } else {
        std::cerr << "Registration error\n";
        return false;
    }
}