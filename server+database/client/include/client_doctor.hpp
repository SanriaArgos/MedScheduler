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

private:
    int doctor_id;
};

}  // namespace doctor

<<<<<<< HEAD
#endif  // DOCTOR_CLIENT_HPP
=======
#endif  // DOCTOR_CLIENT_HPP
>>>>>>> origin/QtApp
