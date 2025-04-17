#ifndef SENIOR_ADMIN_MENU_HPP_
#define SENIOR_ADMIN_MENU_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция меню старшего администратора принимает JSON с необходимыми параметрами.
bool senior_admin_menu(const nlohmann::json &data);

#endif  // SENIOR_ADMIN_MENU_HPP_
