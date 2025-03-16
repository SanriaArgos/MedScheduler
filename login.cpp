#include "login.hpp"
#include "utils.hpp"
#include <iostream>

std::string login(database_handler &db) {
    std::cout << "\n=== Login ===\n";
    std::string phone = get_validated_phone();
    std::string password = get_validated_input("Password", false);
    return db.login_user(phone, password);
}
