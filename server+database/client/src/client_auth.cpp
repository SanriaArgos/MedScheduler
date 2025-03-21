#include "client_auth.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace auth {

using json = nlohmann::json;

user_info login(const std::string& phone, const std::string& password) {
    json login_data = {
        {"phone", phone},
        {"password", password}
    };

    std::string json_data = login_data.dump();
    std::string url = "http://localhost:3000/login";
    std::string response = send_post_request(url, json_data);

    try {
        json response_data = json::parse(response);
        if (response_data.contains("id") && response_data.contains("user_type")) {
            return {response_data["id"], response_data["user_type"]};
        } else {
            std::cerr << "Error: " << response_data.value("error", "Login failed") << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing server response: " << e.what() << std::endl;
    }

    return {-1, ""}; // Возвращаем пустые данные в случае ошибки
}

user_info register_user(const std::string& phone, const std::string& password, const std::string& user_type) {
    json register_data = {
        {"phone", phone},
        {"password", password},
        {"user_type", user_type}
    };

    std::string json_data = register_data.dump();
    std::string url = "http://localhost:3000/register";
    std::string response = send_post_request(url, json_data);

    try {
        json response_data = json::parse(response);
        if (response_data.contains("id") && response_data.contains("user_type")) {
            return {response_data["id"], response_data["user_type"]};
        } else {
            std::cerr << "Error: " << response_data.value("error", "Registration failed") << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing server response: " << e.what() << std::endl;
    }

    return {-1, ""}; // Возвращаем пустые данные в случае ошибки
}

} // namespace auth