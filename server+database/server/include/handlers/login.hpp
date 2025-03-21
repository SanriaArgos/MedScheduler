#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include "../database.hpp"
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция логина принимает JSON с полями: phone, password.
// Возвращает JSON-объект с результатом (например, { "success": true, "result": "patient:123" })
nlohmann::json login(const nlohmann::json &data);

#endif  // LOGIN_HPP_