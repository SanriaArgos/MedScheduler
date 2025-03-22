#ifndef ADD_HOSPITAL_HPP_
#define ADD_HOSPITAL_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>

#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция добавления больницы принимает JSON с полями: region, settlement_type, settlement_name, street, house, full_name, admin_id.
void add_hospital(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // ADD_HOSPITAL_HPP_