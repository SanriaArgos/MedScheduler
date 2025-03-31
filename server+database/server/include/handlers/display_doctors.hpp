#ifndef DISPLAY_DOCTORS_HPP_
#define DISPLAY_DOCTORS_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void display_doctors_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // DISPLAY_DOCTORS_HPP_