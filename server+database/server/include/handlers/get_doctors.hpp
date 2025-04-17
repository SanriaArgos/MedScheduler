#ifndef GET_DOCTORS_HPP_
#define GET_DOCTORS_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_doctors_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // GET_DOCTORS_HPP_