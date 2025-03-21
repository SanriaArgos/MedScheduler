#ifndef ADD_JUNIOR_ADMIN_HPP_
#define ADD_JUNIOR_ADMIN_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция добавления младшего администратора принимает JSON с полями: last_name, first_name, [patronymic], phone.
bool add_junior_admin(const nlohmann::json &data);

#endif  // ADD_JUNIOR_ADMIN_HPP_