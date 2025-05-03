#ifndef CLIENT_PATIENT_HPP
#define CLIENT_PATIENT_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace patient {

using json = nlohmann::json;

class patient_client {
public:
    patient_client(int user_id);

private:
    int user_id;
    void view_doctor_schedule(json request_data);
    void post_doctor_feedback_client(json request_data);
};

}  // namespace patient

#endif  // CLIENT_PATIENT_HPP
