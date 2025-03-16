#include "registration.hpp"
#include "utils.hpp"
#include <iostream>

void registration(database_handler &db) {
    std::cout << "\n=== Registration ===\n";
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
    std::string password = get_validated_password();
    if (db.register_user(last_name, first_name, patronymic, phone, password))
        std::cout << "Registration successful\n";
    else
        std::cout << "Registration error\n";
}