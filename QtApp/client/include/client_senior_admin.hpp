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

    json edit_senior_admin_profile(const json &request_data);

    json get_senior_admin_profile_by_id(int user_id);

private:
    int admin_id;
};

}  // namespace senior_admin

#endif  // CLIENT_SENIOR_ADMIN_HPP
