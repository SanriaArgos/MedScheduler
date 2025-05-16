#ifndef CLIENT_SENIOR_ADMIN_HPP
#define CLIENT_SENIOR_ADMIN_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace senior_admin {

using json = nlohmann::json;

class senior_admin_client {
public:
    senior_admin_client(int admin_id);

    void add_junior_admin(const json &data);
    void add_hospital(const json &data);
    json get_hospitals_table();
    json get_users_table();

    json delete_user_by_phone(const std::string &user_phone);

private:
    int admin_id;
};

}  // namespace senior_admin

#endif  // CLIENT_SENIOR_ADMIN_HPP
