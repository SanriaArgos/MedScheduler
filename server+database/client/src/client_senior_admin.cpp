#include "../include/client_senior_admin.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "../include/common_for_all.hpp"

namespace senior_admin {

using json = nlohmann::json;

senior_admin_client::senior_admin_client(int admin_id) : admin_id(admin_id) {
};

void senior_admin_client::add_junior_admin(const json &json_data) {
    std::string url = "http://localhost:8080/add_junior_admin";
    std::string response = send_post_request(url, json_data);
}

void senior_admin_client::add_hospital(const json &json_data) {
    std::string url = "http://localhost:8080/add_hospital";
    std::string response = send_post_request(url, json_data);
}

json senior_admin_client::get_users_table() {
    std::string url = "http://localhost:8080/get_users";
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching users table: " << e.what() << std::endl;
        return json();
    }
}

json senior_admin_client::get_hospitals_table() {
    std::string url = "http://localhost:8080/get_hospitals";
    std::string response = send_get_request(url);

    try {
        json hospitals = json::parse(response);
        return hospitals;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching hospitals table: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_users_table() {
    std::string url = "http://localhost:8080/get_users";
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching users table: " << e.what() << std::endl;
        return json();
    }
}

}  // namespace senior_admin