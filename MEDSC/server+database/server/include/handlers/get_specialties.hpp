#ifndef GET_SPECIALTIES_HPP_
#define GET_SPECIALTIES_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/specialties
void get_specialties(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_SPECIALTIES_HPP_
