#ifndef USER_MENU_HPP_
#define USER_MENU_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция личного кабинета пациента теперь принимает JSON с необходимыми параметрами.
bool user_menu(const nlohmann::json &data);

#endif  // USER_MENU_HPP_
