#ifndef DOCTOR_MENU_HPP_
#define DOCTOR_MENU_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция меню врача принимает JSON с необходимыми параметрами.
bool doctor_menu(const nlohmann::json &data);

#endif  // DOCTOR_MENU_HPP_
