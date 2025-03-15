#include "login.hpp"
#include "utils.hpp"
#include <iostream>

std::string login(database_handler &db) {
    std::cout << "Вход в личный кабинет:\n";
    std::string phone = get_validated_phone();
    std::string password = get_validated_input("Пароль", false);
    return db.login_user(phone, password);
}
