#ifndef REGISTRATION_HPP_
#define REGISTRATION_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void registration(
    const nlohmann::json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // REGISTRATION_HPP_