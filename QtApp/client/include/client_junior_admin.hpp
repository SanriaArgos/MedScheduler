#ifndef CLIENT_JUNIOR_ADMIN_HPP
#define CLIENT_JUNIOR_ADMIN_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace junior_admin {

using json = nlohmann::json;

class junior_admin_client {
public:
    junior_admin_client(int admin_id);

    void add_doctor(const json &data);
    void add_record_slot(const json &data);
    void attach_doctor_to_hospital_class(const json &data);
    void detach_doctor_from_hospital(const json &data);
    bool check_doctor_exists(int doctor_id);
    bool check_hospital_exists(int hospital_id);
    json get_doctors_table();
    json get_users_table();
    json get_doctor_schedule(int doctor_id);

    bool is_doctor_attached_to_hospital(int doctor_id, int hospital_id);

    json get_waitlist(int doctor_id, int junior_admin_id);

    json delete_doctor_feedback(const json &data);

    json edit_junior_admin_profile(const json &data);

    json get_doctor_statistics(const json &data);

    json get_junior_admin_profile_by_id(int user_id);

private:
    int admin_id;
};

}  // namespace junior_admin

#endif  // CLIENT_JUNIOR_ADMIN_HPP
