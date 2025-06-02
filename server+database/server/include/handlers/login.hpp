#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция логина принимает JSON с полями: phone, password.
// Возвращает JSON-объект с результатом (например, { "success": true, "result":
// "patient:123" })
void login(
    const nlohmann::json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // LOGIN_HPP_