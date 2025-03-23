#ifndef REGISTRATION_HPP_
#define REGISTRATION_HPP_

#include "../database.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void registration(const nlohmann::json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // REGISTRATION_HPP_