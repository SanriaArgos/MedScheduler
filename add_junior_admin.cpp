#include "add_junior_admin.hpp"
#include "utils.hpp"
#include <iostream>
#include <libpq-fe.h>

extern database_handler* global_db;

bool add_junior_admin(const nlohmann::json &data) {
    if (!data.contains("last_name") || !data.contains("first_name") || !data.contains("phone")) {
        std::cerr << "Error: Missing required fields for adding junior admin\n";
        return false;
    }
    std::string last_name = data["last_name"];
    std::string first_name = data["first_name"];
    std::string patronymic = data.value("patronymic", "");
    std::string phone = data["phone"];
    if (global_db->user_exists(phone)) {
        std::cerr << "Error: Phone already registered\n";
        return false;
    }
    const std::string default_password = "0987654321";
    if (!global_db->register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cerr << "Error adding junior administrator\n";
        return false;
    }
    const char* params[1] = { phone.c_str() };
    PGresult *res = PQexecParams(global_db->get_connection(),
        "UPDATE users SET user_type = 'junior administrator' WHERE phone = $1",
        1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating user type: " << PQerrorMessage(global_db->get_connection()) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    std::cerr << "Junior administrator added\n";
    return true;
}
