#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
using json = nlohmann::json;

void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res);

#endif // HANDLERS_HPP