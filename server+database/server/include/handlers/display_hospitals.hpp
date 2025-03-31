#ifndef DISPLAY_HOSPITALS_HPP
#define DISPLAY_HOSPITALS_HPP

#include <nlohmann/json.hpp>
#include "../database.hpp"
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void display_hospitals_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif // DISPLAY_HOSPITALS_HPP