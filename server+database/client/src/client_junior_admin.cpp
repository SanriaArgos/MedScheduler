#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include "client_junior_admin.hpp"
#include "common_for_all.hpp"

using json = nlohmann::json;

void add_doctor(const json& data) {
    std::string json_data = data.dump();
    std::string url = "http://localhost:3000/add_doctor";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

void add_record_slot(const json& data) {
    std::string json_data = data.dump();
    std::string url = "http://localhost:3000/add_record_slot";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

bool check_doctor_exists(int doctor_id) {
    std::string url = "http://localhost:3000/doctors/" + std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        json doctor_data = json::parse(response);
        if (doctor_data.contains("doctor_id")) {
            return true; 
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking doctor: " << e.what() << std::endl;
    }
    return false; 
}

bool check_hospital_exists(int hospital_id) {
    std::string url = "http://localhost:3000/hospitals/" + std::to_string(hospital_id);
    std::string response = send_get_request(url);

    try {
        json hospital_data = json::parse(response);
        if (hospital_data.contains("hospital_id")) {
            return true; 
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking hospital: " << e.what() << std::endl;
    }
    return false; 
}

void attach_doctor_to_hospital(const json& data) {
    int doctor_id = data.value("doctor_id", -1);
    int hospital_id = data.value("hospital_id", -1);

    if (!check_doctor_exists(doctor_id)) {
        std::cerr << "Error: Doctor with ID " << doctor_id << " not found.\n";
        return;
    }

    if (!check_hospital_exists(hospital_id)) {
        std::cerr << "Error: Hospital with ID " << hospital_id << " not found.\n";
        return;
    }

    std::string json_data = data.dump();
    std::string url = "http://localhost:3000/attach_doctor_to_hospital";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

void detach_doctor_from_hospital(const json& data) {
    int doctor_id = data.value("doctor_id", -1);
    int hospital_id = data.value("hospital_id", -1);

    if (!check_doctor_exists(doctor_id)) {
        std::cerr << "Error: Doctor with ID " << doctor_id << " not found.\n";
        return;
    }

    if (!check_hospital_exists(hospital_id)) {
        std::cerr << "Error: Hospital with ID " << hospital_id << " not found.\n";
        return;
    }

    std::string json_data = data.dump();
    std::string url = "http://localhost:3000/detach_doctor_from_hospital";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

json get_doctors_table() {
    std::string url = "http://localhost:3000/doctors";
    std::string response = send_get_request(url);

    try {
        json doctors = json::parse(response);
        return doctors; // Return JSON with doctors data
    } catch (const std::exception& e) {
        std::cerr << "Error fetching doctors table: " << e.what() << std::endl;
        return json(); // Return empty JSON on error
    }
}

json get_users_table() {
    std::string url = "http://localhost:3000/users";
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        return users; // Return JSON with users data
    } catch (const std::exception& e) {
        std::cerr << "Error fetching users table: " << e.what() << std::endl;
        return json(); // Return empty JSON on error
    }
}

json get_doctor_schedule(int doctor_id, int admin_id) {
    std::string check_url = "http://localhost:3000/check_doctor_admin_hospital?doctor_id=" 
                            + std::to_string(doctor_id) + "&admin_id=" + std::to_string(admin_id);
    std::string check_response = send_get_request(check_url);

    try {
        json check_result = json::parse(check_response);
        if (!check_result.value("is_valid", false)) {
            std::cerr << "Error: Doctor and admin are not associated with the same hospital.\n";
            return json(); // Return empty JSON on error
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking hospital association: " << e.what() << std::endl;
        return json(); // Return empty JSON on error
    }

    // Fetch doctor's schedule
    std::string schedule_url = "http://localhost:3000/doctor_schedule?doctor_id=" + std::to_string(doctor_id);
    std::string schedule_response = send_get_request(schedule_url);

    try {
        json schedule = json::parse(schedule_response);
        return schedule; // Return JSON with doctor's schedule
    } catch (const std::exception& e) {
        std::cerr << "Error fetching doctor's schedule: " << e.what() << std::endl;
        return json(); // Return empty JSON on error
    }
}