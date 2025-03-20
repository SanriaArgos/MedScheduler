#ifndef CLIENT_SENIOR_ADMIN_HPP
#define CLIENT_SENIOR_ADMIN_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace senior_admin {

using json = nlohmann::json;

class senior_admin_client {
public:
    senior_admin_client(int admin_id);

    void add_junior_admin(const json& data);
    void add_hospital(const json& data);
    void display_hospitals_table();
    void display_users_table();

    void run_menu(); // Метод для запуска меню

private:
    int admin_id;
};

} // namespace senior_admin

#endif // CLIENT_SENIOR_ADMIN_HPP