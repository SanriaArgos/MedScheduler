#ifndef AUTH_HPP_
#define AUTH_HPP_

#include "handlers.hpp"
#include <boost/beast/http.hpp>
#include <string>

namespace http = boost::beast::http;

namespace Auth {
std::string trim_Whitespace(const std::string &str);

bool validate_Phone(const std::string &phone);
}  // namespace Auth

#endif  // AUTH_HPP_