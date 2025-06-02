#ifndef GET_SETTLEMENT_NAMES_HPP_
#define GET_SETTLEMENT_NAMES_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/settlement_names
void get_settlement_names(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_SETTLEMENT_NAMES_HPP_
