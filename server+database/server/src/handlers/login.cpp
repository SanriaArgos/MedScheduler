#include "../../include/handlers/login.hpp"
#include "../../include/database.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

extern database_handler* global_db;  // Добавляем объявление глобального указателя

nlohmann::json login(const nlohmann::json &data) {
    nlohmann::json response;
    if (!data.contains("phone") || !data.contains("password")) {
        std::cerr << "Login error: missing required fields\n";
        response["success"] = false;
        return response;
    }
    std::string phone = data["phone"];
    std::string password = data["password"];
    std::string login_result = global_db->login_user(phone, password);
    bool success = !login_result.empty();
    response["success"] = success;
    response["action"] = "login";
    if (success) {
        response["result"] = login_result;
    }
    return response;
}