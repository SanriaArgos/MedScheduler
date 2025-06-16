#ifndef DOCTOR_CLIENT_HPP
#define DOCTOR_CLIENT_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace doctor {

using json = nlohmann::json;

class doctor_client {
public:
    doctor_client(int doctor_id);
    json get_schedule();
    json delete_self_account(int doctor_id);

    json edit_doctor_profile(const json &request_data);

    json get_doctor_profile_by_id(int user_id);

private:
    int doctor_id;
};
}  // namespace doctor

#endif  // DOCTOR_CLIENT_HPP
