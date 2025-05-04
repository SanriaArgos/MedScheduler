#ifndef GET_SPECIALTIES_HPP_
#define GET_SPECIALTIES_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/specialties
void get_specialties(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_SPECIALTIES_HPP_
