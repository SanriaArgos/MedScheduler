#include "client_patient.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace patient {

using json = nlohmann::json;

patient_client::patient_client(int user_id) : user_id(user_id) {
}

void patient_client::post_doctor_feedback_client(json request_data) {
    std::string response = send_post_request("http://localhost:8080/post_doctor_rating", request_data);
}

void patient_client::view_doctor_schedule(json request_data) {
    // std::string region, settlement_type, settlement_name, specialty;
    // int hospital_id, doctor_id; все это должно быть в request_data

    std::string response = send_post_request(
        "http://localhost:8080/view_doctor_schedule_for_patient", request_data
    );

    // if (response.empty()) {
    //     std::cerr << "Error: Empty response from server\n";
    //     return;
    // }  тут надо научиться выводить, что запрашиваемых данных нет, и наверное не уводить это в ошибку...

    try {
        json schedule = json::parse(response);
        if (!schedule["success"]) {
            std::cerr << "Error: " << schedule["error"] << std::endl;
            return;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error fetching schedule: " << e.what() << std::endl;
    }
}

}  // namespace patient
