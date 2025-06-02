#include "client_patient.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace patient {

using json = nlohmann::json;

patient_client::patient_client(int user_id) : user_id_(user_id) {
}

json patient_client::get_doctors_for_patient(const json &request_data) {
    try {  // тут если чего-то в переданном джейсоне нет, то устанавливаются
           // пустые кавычки
        std::string region = request_data.value("region", "");
        std::string settlement_type = request_data.value("settlement_type", "");
        std::string settlement_name = request_data.value("settlement_name", "");
        std::string specialty = request_data.value("specialty", "");
        std::string hospital = request_data.value("hospital", "");
        std::string doctor = request_data.value("doctor", "");

        std::string url = "http://localhost:8080/get_doctors_for_patient";
        url += "?region=" + region + "&settlement_type=" + settlement_type +
               "&settlement_name=" + settlement_name +
               "&specialty=" + specialty + "&hospital=" + hospital +
               "&doctor=" + doctor;

        std::string response = send_get_request(url);
        json json_response = json::parse(response);

        if (!json_response.contains("success")) {
            json_response["success"] = false;
            json_response["error"] = "Missing field 'success'";
        }
        return json_response;
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON or error while processing: ") +
                          e.what()}};
    }
}

json patient_client::get_regions() {
    try {
        std::string url = "http://localhost:8080/get_regions";
        std::string response = send_get_request(url);

        json json_response = json::parse(response);

        if (!json_response.contains("success")) {
            json_response["success"] = false;
            json_response["error"] = "Missing field 'success'";
        }
        return json_response;
    }

    catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON or error while processing: ") +
                          e.what()}};
    }
}

json patient_client::get_settlement_types() {
    try {
        std::string url = "http://localhost:8080/get_settlement_types";
        std::string response = send_get_request(url);

        json json_response = json::parse(response);

        if (!json_response.contains("success")) {
            json_response["success"] = false;
            json_response["error"] = "Missing field 'success'";
        }
        return json_response;
    }

    catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON or error while processing: ") +
                          e.what()}};
    }
}

json patient_client::get_settlement_names() {
    try {
        std::string url = "http://localhost:8080/get_settlement_names";
        std::string response = send_get_request(url);

        json json_response = json::parse(response);

        if (!json_response.contains("success")) {
            json_response["success"] = false;
            json_response["error"] = "Missing field 'success'";
        }
        return json_response;
    }

    catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON or error while processing: ") +
                          e.what()}};
    }
}

json patient_client::get_specialties() {
    try {
        std::string url = "http://localhost:8080/get_specialties";
        std::string response = send_get_request(url);
        json json_response = json::parse(response);

        if (!json_response.contains("success")) {
            json_response["success"] = false;
            json_response["error"] = "Missing field 'success'";
        }
        return json_response;
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Error getting specialties: ") + e.what()}};
    }
}

json patient_client::get_hospital_full_names() {
    try {
        std::string url = "http://localhost:8080/get_hospital_full_names";
        std::string response = send_get_request(url);
        json json_response = json::parse(response);

        if (!json_response.contains("success")) {
            json_response["success"] = false;
            json_response["error"] = "Missing field 'success'";
        }
        return json_response;
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Error getting hospitals: ") + e.what()}};
    }
}

json patient_client::get_doctor_schedule_for_patient(int doctor_id) {
    // Формируем URL с параметрами
    std::string url = "http://localhost:8080/get_doctor_schedule_for_patient?";
    url += "doctor_id=" + std::to_string(doctor_id);

    // Отправляем GET-запрос
    std::string response = send_get_request(url);

    try {
        json schedule = json::parse(response);
        if (!schedule.contains("success")) {
            schedule["success"] = false;
            schedule["error"] = "Missing field 'success'";
        }
        return schedule;
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::book_appointment(const json &request_data) {
    std::string url = "http://localhost:8080/book_appointment";
    std::string response = send_post_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::search_doctors(const json &request_data) {
    std::string url = "http://localhost:8080/search_doctors";
    std::string response = send_post_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::patient_appointments(int patient_id) {
    std::string url =
        "http://localhost:8080/get_patient_appointments?patient_id=" +
        std::to_string(patient_id);
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_appointments(const json &request_data) {
    std::string url = "http://localhost:8080/add_patient_to_waitlist";
    std::string response = send_post_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::get_doctor_average_ratings() {
    std::string url = "http://localhost:8080/get_doctor_average_ratings";
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::get_doctor_feedback_items(int doctor_id) {
    std::string url =
        "http://localhost:8080/get_doctor_feedback_items?doctor_id=" +
        std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", "Failed to parse response: " + std::string(e.what())}};
    }
}

json patient_client::get_doctor_feedback_calculated(int doctor_id) {
    std::string url =
        "http://localhost:8080/get_doctor_feedback_calculated?doctor_id=" +
        std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", "Failed to parse response: " + std::string(e.what())}};
    }
}

json patient_client::get_doctor_hospitals(int doctor_id) {
    std::string url = "http://localhost:8080/get_doctor_hospitals?doctor_id=" +
                      std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        json json_doctor_hospitals = json::parse(response);
        if (!json_doctor_hospitals.contains("success")) {
            json_doctor_hospitals["success"] = false;
            json_doctor_hospitals["error"] = "Missing field 'success'";
        }
        return json_doctor_hospitals;
    } catch (...) {
        return {{"success", false}, {"error", "Invalid JSON"}};
    }
}

json patient_client::post_doctor_feedback_client(const json &request_data) {
    std::string url = "http://localhost:8080/post_doctor_feedback";
    std::string response = send_post_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::edit_doctor_feedback(const json &request_data) {
    std::string url = "http://localhost:8080/edit_doctor_feedback";
    std::string response = send_patch_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::cancel_appointment(const json &data) {
    std::string url = "http://localhost:8080/cancel_appointment";
    std::string response = send_patch_request(url, data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::cancel_waitlist(const json &data) {
    std::string url = "http://localhost:8080/cancel_appointment_from_waitlist";
    std::string response = send_post_request(url, data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::delete_self_account(int patient_id) {
    std::string url = "http://localhost:8080/delete_self_account?user_id" +
                      std::to_string(patient_id);
    std::string response = send_delete_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::get_cancelled_slots(int doctor_id) {
    std::string url = "http://localhost:8080/get_cancelled_slots?doctor_id=" +
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

json patient_client::get_waitlist_count(int doctor_id) {
    std::string url = "http://localhost:8080/get_waitlist_count?doctor_id=" +
                      std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json doctor_client::edit_patient_profile(const json &request_data) {
    std::string url = "http://localhost:8080/edit_patient_profile";
    std::string response = send_patch_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

}  // namespace patient
