#include "user_menu.hpp"
#include "patient_schedule.hpp"
#include <iostream>

// Клиентская логика полностью удалена – оставлена заглушка.
bool user_menu(const nlohmann::json &data) {
    std::cerr << "user_menu: client-side menu removed. Use JSON interface instead.\n";
    return false;
}
