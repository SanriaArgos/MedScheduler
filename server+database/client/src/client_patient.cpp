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

        std::string url =
            "http://localhost:8080/get_doctors_for_patient";
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

json patient_client::get_doctor_schedule_for_patient(int doctor_id, int hospital_id) {
    // Формируем URL с параметрами
    std::string url = "http://localhost:8080/get_doctor_schedule_for_patient?";
    url += "doctor_id=" + std::to_string(doctor_id);
    url += "&hospital_id=" + std::to_string(hospital_id);

    // Отправляем GET-запрос
    std::string resp = send_get_request(url);

    try {
        json j = json::parse(resp);
        if (!j.contains("success")) {
            j["success"] = false;
            j["error"] = "Missing field 'success'";
        }
        return j;
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}
        };
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
    std:string response = send_post_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json patient_client::patient_appointments(int patient_id) {
    std::string url = "http://localhost:8080/get_patient_appointments" + std::to_string(patinet_id);
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

json patient_client::post_doctor_feedback_client(const json &request_data) {
    std::string url = "http://localhost:8080/post_doctor_rating";
    std::string response = send_post_request(
        url, request_data
    );

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

}  // namespace patient
