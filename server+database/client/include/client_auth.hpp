#ifndef CLIENT_AUTH_HPP
#define CLIENT_AUTH_HPP

#include <string>

namespace auth {

struct user_info {
    int id;
    std::string user_type;
};

user_info login(const std::string& phone, const std::string& password);
user_info register_user(const std::string& phone, const std::string& password, const std::string& first_name, const std::string& last_name, const std::string& patronymic);

} // namespace auth

#endif // CLIENT_AUTH_HPP