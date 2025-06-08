#ifndef GET_ADMIN_HOSPITAL_HPP_
#define GET_ADMIN_HOSPITAL_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_admin_hospital(int admin_id, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // GET_ADMIN_HOSPITAL_HPP_
