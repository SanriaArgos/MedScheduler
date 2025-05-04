#ifndef CLIENT_PATIENT_HPP
#define CLIENT_PATIENT_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace patient {

    using json = nlohmann::json;
    
    class patient_client {
    public:
        explicit patient_client(int user_id);
    
        // POST /view_doctor_schedule_for_patient
        // { "hospital_id":<int>, "doctor_id":<int> }
        // → { "success":…, "schedule":[…] }
        json view_doctor_schedule(const json &request_data);
    
        // POST /post_doctor_rating
        // { "doctor_ref_id", "text", "name", "date", "rate", "address" }
        // → { "success": true, "id":… } или error
        json post_doctor_feedback_client(const json &request_data);
    
    private:
        int user_id_;
    };
    
    }  // namespace patient
    
    #endif  // CLIENT_PATIENT_HPP_