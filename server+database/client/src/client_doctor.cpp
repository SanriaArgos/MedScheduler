#include "client_doctor.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace doctor {

using json = nlohmann::json;

doctor_client::doctor_client(int doctor_id) : doctor_id(doctor_id) {
}

json doctor_client::get_schedule() {
    std::string url =
        "http://localhost:8080/get_doctor_schedule?doctor_id=" + std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        json full_response = json::parse(response);
        return full_response;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching doctor_schedule: " << e.what() << std::endl;
        return json();
    }
}

}  // namespace doctor
