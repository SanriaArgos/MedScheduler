#ifndef CLIENT_PATIENT_HPP
#define CLIENT_PATIENT_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace patient {

using json = nlohmann::json;

class patient_client {
public:
    explicit patient_client(int user_id);

    json get_doctors_for_patient(const json &request_data);

    // GET /doctors/{doctor_id}/clinics
    // json get_doctor_clinics(int doctor_id);

    // POST /post_doctor_rating
    // { "doctor_ref_id", "text", "name", "date", "rate", "address" }
    // → { "success": true, "id":… } или error
    json post_doctor_feedback_client(const json &request_data);

    json get_regions();
    json get_settlemet_types();
    json get_settlemet_names();
    json get_hospital_full_names();
    json get_specialties();

    json get_doctor_average_ratings();
    json get_doctor_feedback_items(int doctor_id);
    json get_doctor_feedback_calculated(int doctor_id)
    json get_doctor_clinics(int doctor_id);

private:
    int user_id_;
};

}  // namespace patient

#endif  // CLIENT_PATIENT_HPP_