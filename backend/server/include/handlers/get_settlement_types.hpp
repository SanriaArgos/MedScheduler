#ifndef GET_SETTLEMENT_TYPES_HPP_
#define GET_SETTLEMENT_TYPES_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/settlement_types
void get_settlement_types(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_SETTLEMENT_TYPES_HPP_
