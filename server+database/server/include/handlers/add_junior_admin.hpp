#ifndef ADD_JUNIOR_ADMIN_HPP_
#define ADD_JUNIOR_ADMIN_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция добавления младшего администратора принимает JSON с полями: last_name, first_name, [patronymic], phone.
void add_junior_admin(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // ADD_JUNIOR_ADMIN_HPP_