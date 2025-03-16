#include "doctor_client.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

void display_doctor_schedule(int doctor_id) {
    std::string url = "http://localhost:3000/doctor_schedule?doctor_id=" + std::to_string(doctor_id);
    std::string response = send_get_request(url);

    try {
        json schedule = json::parse(response);

        std::cout << "\n=== Doctor's Schedule ===\n";
        std::string current_date;
        for (const auto& entry : schedule) {
            std::string date = entry["appointment_date"];
            std::string time = entry["appointment_time"];
            std::string hospital = entry["full_name"];
            std::string cabinet = entry["cabinet_number"];
            std::string last_name = entry["last_name"];
            std::string first_name = entry["first_name"];
            std::string patronymic = entry["patronymic"];
            std::string patient_phone = entry["phone"];

            if (date != current_date) {
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }

            std::cout << time << ": " << hospital << ", cabinet " << cabinet;
            if (last_name.empty() && first_name.empty() && patronymic.empty()) {
                std::cout << ", free";
            } else {
                std::cout << ", " << last_name << " " << first_name << " " << patronymic
                          << ", " << patient_phone;
            }
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}