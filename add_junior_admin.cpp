#include "add_junior_admin.hpp"
#include "utils.hpp"
#include <iostream>
#include <libpq-fe.h>

extern database_handler* global_db;

bool add_junior_admin(const std::string &last_name,
                      const std::string &first_name,
                      const std::string &patronymic,
                      const std::string &phone) {
    if (global_db->user_exists(phone)) {
        std::cerr << "Error: Phone already registered\n";
        return false;
    }
    const std::string default_password = "0987654321";
    if (!global_db->register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cerr << "Error adding junior administrator\n";
        return false;
    }
    const char* paramValues[1] = { phone.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "UPDATE users SET user_type = 'junior administrator' WHERE phone = $1",
        1, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating user type: " << PQerrorMessage(global_db->get_connection()) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    std::cerr << "Junior administrator added\n";
    return true;
}
