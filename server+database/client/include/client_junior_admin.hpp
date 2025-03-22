#ifndef CLIENT_JUNIOR_ADMIN_HPP
#define CLIENT_JUNIOR_ADMIN_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace junior_admin {

using json = nlohmann::json;

class junior_admin_client {
public:
    junior_admin_client(int admin_id);

    void add_doctor(const json& data);
    void add_record_slot(const json& data);
    void attach_doctor_to_hospital(const json& data);
    void detach_doctor_from_hospital(const json& data);
    bool check_doctor_exists(int doctor_id);
    bool check_hospital_exists(int hospital_id);
    json get_doctors_table();
    json get_users_table();
    json get_doctor_schedule(int doctor_id);

    bool is_doctor_attached_to_hospital(int doctor_id, int hospital_id);
    
    void run_menu(); // Добавляем метод для запуска меню
    
private:
    int admin_id;
};

} // namespace junior_admin

#endif // CLIENT_JUNIOR_ADMIN_HPP