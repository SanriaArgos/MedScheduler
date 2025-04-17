#ifndef GET_USERS_HPP_
#define GET_USERS_HPP_

#include <nlohmann/json.hpp>
#include "../database.hpp"
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_users_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // GET_USERS_HPP_