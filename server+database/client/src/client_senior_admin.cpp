#include "client_senior_admin.hpp"
#include "common_for_all.hpp"
#include <iostream>

void add_junior_admin(const json& data) {
    std::string json_data = data.dump();
    std::string url = "http://localhost:3000/add_junior_admin";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

void add_hospital(const json& data) {
    std::string json_data = data.dump();
    std::string url = "http://localhost:3000/add_hospital";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

void display_hospitals_table() {
    std::string url = "http://localhost:3000/hospitals";
    std::string response = send_get_request(url);

    try {
        json hospitals = json::parse(response);
        std::cout << "\n=== Hospitals Table ===\n";
        for (const auto& hospital : hospitals) {
            std::cout << "(" << hospital["hospital_id"] << ") "
                      << hospital["region"] << "-"
                      << hospital["settlement_type"] << "-"
                      << hospital["settlement_name"] << "-"
                      << hospital["street"] << "-"
                      << hospital["house"] << "-"
                      << hospital["full_name"] << "-"
                      << hospital["administrator_id"] << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error fetching hospitals table: " << e.what() << std::endl;
    }
}

void display_users_table() {
    std::string url = "http://localhost:3000/users";
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        std::cout << "\n=== Users Table ===\n";
        for (const auto& user : users) {
            std::cout << "(" << user["id"] << ") "
                      << user["last_name"] << "-"
                      << user["first_name"] << "-"
                      << user["patronymic"] << ", "
                      << user["phone"] << ", "
                      << user["user_type"] << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error fetching users table: " << e.what() << std::endl;
    }
}