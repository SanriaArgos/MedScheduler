#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include "../database.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция логина принимает JSON с полями: phone, password.
// Возвращает JSON-объект с результатом (например, { "success": true, "result": "patient:123" })
void login(const nlohmann::json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // LOGIN_HPP_