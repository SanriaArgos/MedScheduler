#ifndef GET_HOSPITALS_HPP
#define GET_HOSPITALS_HPP

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_hospitals_table(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_HOSPITALS_HPP