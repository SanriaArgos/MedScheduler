#ifndef GET_HOSPITAL_FULL_NAMES_HPP_
#define GET_HOSPITAL_FULL_NAMES_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/hospital_full_names
void get_hospital_full_names(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_HOSPITAL_FULL_NAMES_HPP_
