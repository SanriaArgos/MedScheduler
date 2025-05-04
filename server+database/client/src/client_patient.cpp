#include "client_patient.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace patient {

    using json = nlohmann::json;
    
    patient_client::patient_client(int user_id)
        : user_id_(user_id) { }
    
    json patient_client::view_doctor_schedule(const json &request_data) {
        std::string resp = send_post_request(
            "http://localhost:8080/view_doctor_schedule_for_patient",
            request_data
        );
        try {
            json j = json::parse(resp);
            if (!j.contains("success")) {
                j["success"] = false;
                j["error"]   = "Missing field 'success'";
            }
            return j;
        } catch (const std::exception &e) {
            return {
                {"success", false},
                {"error", std::string("Invalid JSON from server: ") + e.what()}
            };
        }
    }
    
    json patient_client::get_doctor_clinics(int doctor_id) {
        std::string url = "http://localhost:8080/doctors/" +
                          std::to_string(doctor_id) +
                          "/clinics";
        std::string resp = send_get_request(url);
        try {
            json j = json::parse(resp);
            if (!j.contains("success")) {
                j["success"] = false;
                j["error"]   = "Missing field 'success'";
            }
            return j;
        } catch (...) {
            return {{"success", false}, {"error", "Invalid JSON"}};
        }
    }
    
    json patient_client::post_doctor_feedback_client(const json &request_data) {
        std::string resp = send_post_request(
            "http://localhost:8080/post_doctor_rating",
            request_data
        );
        try {
            return json::parse(resp);
        } catch (const std::exception &e) {
            return {
                {"success", false},
                {"error", std::string("Invalid JSON from server: ") + e.what()}
            };
        }
    }
    
    }  // namespace patient
