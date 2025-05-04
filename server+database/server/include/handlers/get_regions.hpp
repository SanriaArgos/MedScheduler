#ifndef GET_REGIONS_HPP_
#define GET_REGIONS_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/regions
void get_regions(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_REGIONS_HPP_
