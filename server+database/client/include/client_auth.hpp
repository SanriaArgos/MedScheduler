#ifndef CLIENT_AUTH_HPP
#define CLIENT_AUTH_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace auth {

using json = nlohmann::json;

struct user_info {
    int id;
    std::string user_type;
};

user_info login(const std::string& phone, const std::string& password);

} // namespace auth

#endif // CLIENT_AUTH_HPP