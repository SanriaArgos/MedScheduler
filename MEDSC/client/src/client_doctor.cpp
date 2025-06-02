#include "client_doctor.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace doctor {

using json = nlohmann::json;

doctor_client::doctor_client(int doctor_id) : doctor_id(doctor_id) {
}

json doctor_client::get_schedule() {
    std::string url = "http://localhost:8080/get_doctor_schedule?doctor_id=" +
                      std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        json schedule = json::parse(response);
        return schedule;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching doctor_schedule: " << e.what()
                  << std::endl;
        return json();
    }
}

json doctor_client::delete_self_account(int doctor_id) {
    std::string url = "http://localhost:8080/delete_self_account?user_id" +
                      std::to_string(doctor_id);
    std::string response = send_delete_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json doctor_client::edit_doctor_profile(const json &request_data) {
    std::string url = "http://localhost:8080/edit_doctor_profile";
    std::string response = send_patch_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

}  // namespace doctor
