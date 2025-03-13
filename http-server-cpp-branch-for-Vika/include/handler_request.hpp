#ifndef HANDLERS_HPP_
#define HANDLERS_HPP_

#include "handlers.hpp"
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
using json = nlohmann::json;

void handle_request(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    Database_Handler &db_handler
);

#endif  // HANDLERS_HPP