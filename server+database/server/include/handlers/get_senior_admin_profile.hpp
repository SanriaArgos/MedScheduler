#ifndef GET_SENIOR_ADMIN_PROFILE_HPP_
#define GET_SENIOR_ADMIN_PROFILE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_senior_admin_profile(int user_id, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // GET_SENIOR_ADMIN_PROFILE_HPP_
