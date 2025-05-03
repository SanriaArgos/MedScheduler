#include "client_auth.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace auth {

using json = nlohmann::json;

user_info login(const std::string &phone, const std::string &password) {
    json login_data = {{"phone", phone}, {"password", password}};

    std::string url = "http://localhost:8080/login";

    std::string response;
    try {
        response = send_post_request(url, login_data);
    } catch (const std::exception &e) {
        std::cerr << "Error sending login request: " << e.what() << "\n";
        return {-1, ""};
    }
  
    json response_j;
    try {
        response_j = nlohmann::json::parse(response);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing login response: " << e.what() << "\n";
        return {-1, ""};
    }

    if (!response_j.value("success", false)) {
        std::cerr << "Login failed: " << response_j.value("error", "Unknown error") << "\n";
        return {-1, ""};
    }

    // Извлекаем поле "result", например "patient:42"
    std::string result = response_j.value("result", "");
    size_t sep = result.find(':');
    if (sep == std::string::npos) {
        std::cerr << "Invalid result format: " << result << "\n";
        return {-1, ""};
    }

    int user_id = -1;
    try {
        user_id = std::stoi(result.substr(sep + 1));
    } catch (const std::exception &e) {
        std::cerr << "Error parsing user ID: " << e.what() << "\n";
        return {-1, ""};
    }

    // Получаем полный user_type через GET-запрос, используя код в database.cpp
    std::string type_response;
    try {
        type_response = send_get_request(
            "http://localhost:8080/get_user_type?phone=" + phone);
    } catch (const std::exception &e) {
        std::cerr << "Error fetching user type: " << e.what() << "\n";
        return {-1, ""};
    }

    std::string user_type;
    try {
        auto type_json = json::parse(type_response);
        user_type = type_json.value("user_type", "");
    } catch (const std::exception &e) {
        std::cerr << "Error parsing user type response: " << e.what() << "\n";
        return {-1, ""};
    }

    return {user_id, user_type};
}

user_info register_user(
    const std::string &phone,
    const std::string &password,
    const std::string &first_name,
    const std::string &last_name,
    const std::string &patronymic
) {
    json register_data = {{"phone", phone},           {"password", password},
                          {"first_name", first_name}, {"last_name", last_name},
                          {"patronymic", patronymic}, {"user_type", "patient"}};

    std::string response =
        send_post_request("http://localhost:8080/registration", register_data);

    // Проверяем успешность регистрации
    try {
        auto reg_json = nlohmann::json::parse(response);
        if (!reg_json.value("success", false)) {
            std::cerr << "Registration failed: "
                      << reg_json.value("error", "unknown") << "\n";
            return {-1, ""};
        }
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse registration response: " << e.what()
                  << "\n";
        return {-1, ""};
    }

    std::string phone_1 = register_data["phone"];

    // GET ID
    std::string resp_id =
        send_get_request("http://localhost:8080/get_user_id?phone=" + phone_1);
    int id = -1;
    try {
        auto j = nlohmann::json::parse(resp_id);
        id = j.value("id", -1);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing get_user_id response: " << e.what() << "\n";
    }

    // GET user_type
    std::string resp_type = send_get_request(
        "http://localhost:8080/get_user_type?phone=" + phone_1
    );
    std::string user_type;
    try {
        auto j2 = nlohmann::json::parse(resp_type);
        user_type = j2.value("user_type", "");
    } catch (const std::exception &e) {
        std::cerr << "Error parsing get_user_type response: " << e.what()
                  << "\n";
    }

    if (id == -1 || user_type.empty()) {
        std::cerr << "Error retrieving user info: id=" << id << ", user_type=\""
                  << user_type << "\"\n";
        return {-1, ""};
    }

    return {id, user_type};
}

}  // namespace auth
