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

    std::string url = "http://localhost:8080/login";
    std::string response = send_post_request(url, login_data);
    
    std::string phone_1 = login_data["phone"];
    json response_j;
    
    std::string resp_id = send_get_request("http://localhost:8080/get_user_id?phone=" + phone_1);
    int id = -1;
    try {
        response_j = nlohmann::json::parse(resp_id);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing get_user_id response: " << e.what() << "\n";
    }

    std::string resp_type = send_get_request("http://localhost:8080/get_user_type?phone=" + phone_1);
    std::string user_type;
    try {
        auto j2 = nlohmann::json::parse(resp_type);  
        user_type = j2.value("user_type", "");  

    if (!user_type.empty()) {
        response_j["user_type"] = user_type; 
    }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing get_user_type response: " << e.what() << "\n";
    }

    try {
        if (response_j.contains("id") && response_j.contains("user_type")) {
            return {response_j["id"], response_j["user_type"]};
        } else {
            std::cerr << "Error: " << response_j.value("error", "Login failed") << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing server response: " << e.what() << std::endl;
    }

    return {-1, ""}; // Возвращаем пустые данные в случае ошибки
}

user_info register_user(
    const std::string& phone,
    const std::string& password,
    const std::string& first_name,
    const std::string& last_name,
    const std::string& patronymic
) {
    json register_data = {
        {"phone", phone},
        {"password", password},
        {"first_name", first_name},
        {"last_name", last_name},
        {"patronymic", patronymic},
        {"user_type", "patient"}
    };

    std::string response = send_post_request("http://localhost:8080/registration", register_data);

    //Проверяем успешность регистрации
    try {
        auto reg_json = nlohmann::json::parse(response);
        if (!reg_json.value("success", false)) {
            std::cerr << "Registration failed: " << reg_json.value("error", "unknown") << "\n";
            return {-1, ""};
        }
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse registration response: " << e.what() << "\n";
        return {-1, ""};
    }

    std::string phone_1 = register_data["phone"];
    
    // GET ID
    std::string resp_id = send_get_request("http://localhost:8080/get_user_id?phone=" + phone_1);
    int id = -1;
    try {
        auto j = nlohmann::json::parse(resp_id);
        id = j.value("id", -1);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing get_user_id response: " << e.what() << "\n";
    }

    // GET user_type
    std::string resp_type = send_get_request("http://localhost:8080/get_user_type?phone=" + phone_1);
    std::string user_type;
    try {
        auto j2 = nlohmann::json::parse(resp_type);
        user_type = j2.value("user_type", "");
    } catch (const std::exception &e) {
        std::cerr << "Error parsing get_user_type response: " << e.what() << "\n";
    }

    if (id == -1 || user_type.empty()) {
        std::cerr << "Error retrieving user info: id=" << id << ", user_type=\"" << user_type << "\"\n";
        return {-1, ""};
    }

    return { id, user_type };
}

} // namespace auth