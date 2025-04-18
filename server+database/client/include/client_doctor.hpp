#ifndef DOCTOR_CLIENT_HPP
#define DOCTOR_CLIENT_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace doctor {

using json = nlohmann::json;

class doctor_client {
public:
    doctor_client(int doctor_id);
    void get_schedule(int doctor_id);

private:
    int doctor_id;
};

}  // namespace doctor

#endif  // DOCTOR_CLIENT_HPP
