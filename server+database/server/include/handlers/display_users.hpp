#ifndef DISPLAY_USERS_HPP_
#define DISPLAY_USERS_HPP_

#include <nlohmann/json.hpp>
#include "../database.hpp"
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция для получения данных о пользователях в формате JSON
void display_users_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // DISPLAY_USERS_HPP_