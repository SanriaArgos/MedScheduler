#ifndef DISPLAY_USERS_HPP_
#define DISPLAY_USERS_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция возвращает JSON-массив с данными пользователей.
json display_users_table(const json &data);

#endif  // DISPLAY_USERS_HPP_
