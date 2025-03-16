#include "client_senior_admin.hpp"
#include "common_for_all.hpp"
#include <iostream>

void display_menu() {
    std::cout << "\n=== Senior Admin Menu ===\n";
    std::cout << "1. Add junior admin\n";
    std::cout << "2. Add hospital\n";
    std::cout << "3. View hospitals table\n";
    std::cout << "4. View users table\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    int choice = 0;

    while (true) {
        display_menu();
        std::cin >> choice;

        switch (choice) {
            case 1: { //для сведения двух частей пока что такого хватит, думаю
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

            case 2: { //для сведения двух частей пока что такого хватит, думаю
                std::string region, settlement_type, settlement_name, street, house, full_name;
                int administrator_id;
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
                add_hospital(hospital_data);
                break;
            }

            case 3: {
                display_hospitals_table();
            }

            case 4: {
                display_users_table();
            }

            case 5: {
                std::cout << "Exiting the program.\n";
                return 0;
            }

            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}