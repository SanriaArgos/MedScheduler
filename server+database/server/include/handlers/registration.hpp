#ifndef REGISTRATION_HPP_
#define REGISTRATION_HPP_

#include "../database.hpp"
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция регистрации теперь принимает JSON с полями: last_name, first_name, [patronymic], phone, password.
// Возвращает true при успехе, false – при ошибке.
bool registration(const json &data);

#endif  // REGISTRATION_HPP_