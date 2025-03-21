#ifndef DISPLAY_USERS_HPP_
#define DISPLAY_USERS_HPP_

#include <nlohmann/json.hpp>
#include "../database.hpp"

using json = nlohmann::json;

// Функция для получения данных о пользователях в формате JSON
json display_users_table(database_handler &db);

#endif  // DISPLAY_USERS_HPP_