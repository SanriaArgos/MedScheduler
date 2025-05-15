#ifndef GET_CANCELLED_SLOTS_HPP_
#define GET_CANCELLED_SLOTS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_cancelled_slots(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif
