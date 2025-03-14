#ifndef AUTH_HPP_
#define AUTH_HPP_

#include <string>
#include "database.hpp"

namespace Auth {
std::string trim_whitespace(const std::string &str);
bool validate_phone(const std::string &phone);
}  // namespace Auth

#endif  // AUTH_HPP_
