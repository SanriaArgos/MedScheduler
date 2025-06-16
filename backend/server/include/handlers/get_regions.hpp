#ifndef GET_REGIONS_HPP_
#define GET_REGIONS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/regions
void get_regions(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_REGIONS_HPP_
