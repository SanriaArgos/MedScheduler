#include "registration.hpp"
#include <iostream>
#include "utils.hpp"

void registration(database_handler &db) {
    std::cout << "Регистрация пользователя:\n";
    std::string last_name = get_validated_name("Фамилия", true);
    std::string first_name = get_validated_name("Имя", true);
    std::string patronymic = get_input("Отчество (необязательно): ");
    if (!patronymic.empty()) {
        while (!is_cyrillic(patronymic)) {
            std::cout << "Ошибка: используйте только кириллицу\n";
            patronymic = get_input("отчество");
        }
    }
    std::string phone = get_validated_phone();
    while (db.user_exists(phone)) {
        std::cout << "Ошибка: телефон уже зарегистрирован\n";
        phone = get_validated_phone();
    }
    std::string password = get_validated_password();
    if (db.register_user(last_name, first_name, patronymic, phone, password)) {
        std::cout << "Регистрация прошла успешно\n";
    } else {
        std::cout << "Ошибка регистрации\n";
    }
}