#include "client_senior_admin.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace senior_admin {

using json = nlohmann::json;

senior_admin_client::senior_admin_client(int admin_id) : admin_id(admin_id) {}


void senior_admin_client::run_menu() {
    int choice = 0;

    while (true) {
        std::cout << "\n=== Senior Admin Menu ===\n";
        std::cout << "1. Add junior admin\n";
        std::cout << "2. Add hospital\n";
        std::cout << "3. View hospitals table\n";
        std::cout << "4. View users table\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string last_name, first_name, patronymic, phone;
                std::cout << "Enter last name: ";
                std::cin >> last_name;
                std::cout << "Enter first name: ";
                std::cin >> first_name;
                std::cout << "Enter patronymic: ";
                std::cin >> patronymic;
                std::cout << "Enter phone: ";
                std::cin >> phone;

                json junior_admin_data = {
                    {"last_name", last_name},
                    {"first_name", first_name},
                    {"patronymic", patronymic},
                    {"phone", phone}
                };
                add_junior_admin(junior_admin_data);
                break;
            }

            case 2: {
                std::string region, settlement_type, settlement_name, street, house, full_name;
                std::string administrator_id;
                std::cout << "Enter region: ";
                std::cin >> region;
                std::cout << "Enter settlement type: ";
                std::cin >> settlement_type;
                std::cout << "Enter settlement name: ";
                std::cin >> settlement_name;
                std::cout << "Enter street: ";
                std::cin >> street;
                std::cout << "Enter house: ";
                std::cin >> house;
                std::cout << "Enter full hospital name: ";
                std::cin >> full_name;
                std::cout << "Enter admin ID: ";
                std::cin >> administrator_id;

                json hospital_data = {
                    {"region", region},
                    {"settlement_type", settlement_type},
                    {"settlement_name", settlement_name},
                    {"street", street},
                    {"house", house},
                    {"full_name", full_name},
                    {"administrator_id", administrator_id}
                };
                std::cerr << hospital_data["region"] << " " << hospital_data["settlement_type"] << " " << hospital_data["settlement_name"] << " ";
                std::cerr << hospital_data["street"] << " " << hospital_data["house"] << " " << hospital_data["full_name"] << hospital_data["administrator_id"] << std::endl;
                add_hospital(hospital_data);
                break;
            }

            case 3: {
                display_hospitals_table();
                break;
            }

            case 4: {
                display_users_table();
                break;
            }

            case 5: {
                std::cout << "Exiting Senior Admin Menu.\n";
                return;
            }

            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
}

void senior_admin_client::add_junior_admin(const json& json_data) {
    std::string url = "http://localhost:8080/add_junior_admin";
    std::string response = send_post_request(url, json_data);
}

void senior_admin_client::add_hospital(const json& json_data) {
    std::string url = "http://localhost:8080/add_hospital";
    std::string response = send_post_request(url, json_data);
}

void senior_admin_client::display_hospitals_table() {
    std::string url = "http://localhost:8080/display_hospitals";
    std::string response = send_get_request(url);

    try {
        json result = json::parse(response);
        const auto& hospitals = result["hospitals"];
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

void senior_admin_client::display_users_table() {
    std::string url = "http://localhost:8080/display_users";
    std::string response = send_get_request(url);

    try {
        json result = json::parse(response);
        const auto& users = result["users"];
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

} // namespace senior_admin