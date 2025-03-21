#ifndef JUNIOR_ADMIN_MENU_HPP_
#define JUNIOR_ADMIN_MENU_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция меню младшего администратора принимает JSON с необходимыми параметрами.
bool junior_admin_menu(const nlohmann::json &data);

#endif  // JUNIOR_ADMIN_MENU_HPP_
