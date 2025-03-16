#include "add_junior_admin.hpp"
#include "utils.hpp"
#include <iostream>
#include <libpq-fe.h>

void add_junior_admin(database_handler &db) {
    std::cout << "\n=== Add Junior Administrator ===\n";
    std::string last_name = get_validated_name("Last Name", true);
    std::string first_name = get_validated_name("First Name", true);
    std::string patronymic = get_input("Patronymic (optional): ");
    if (!patronymic.empty()) {
        while (!is_latin_or_dash(patronymic)) {
            std::cout << "Error: Use only Latin letters and dash\n";
            patronymic = get_input("Patronymic (optional): ");
        }
    }
    std::string phone = get_validated_phone();
    if (db.user_exists(phone)) {
        std::cout << "Error: Phone already registered\n";
        return;
    }
    const std::string default_password = "0987654321";
    if (!db.register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cout << "Error adding junior administrator\n";
        return;
    }
    std::string query = "UPDATE users SET user_type = 'junior administrator' WHERE phone = '" + phone + "'";
    PGresult *res = PQexec(db.get_connection(), query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating user type: " << PQerrorMessage(db.get_connection()) << "\n";
    }
    PQclear(res);
    std::cout << "Junior administrator added\n";
}
