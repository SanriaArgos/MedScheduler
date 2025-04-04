#include "client_doctor.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace doctor {

using json = nlohmann::json;

doctor_client::doctor_client(int doctor_id) : doctor_id(doctor_id) {
}

void doctor_client::run_menu() {
    int choice = 0;

    while (true) {
        std::cout << "\n=== Doctor Menu ===\n";
        std::cout << "1. View Schedule\n";
        std::cout << "2. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                display_schedule();
                break;
            case 2:
                std::cout << "Exiting Doctor Menu.\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

void doctor_client::display_schedule() {
    int doctor_id;
    std::cout << "Enter your ID: ";
    std::cin >> doctor_id;

    std::string url =
        "http://localhost:8080/display_doctor_schedule?doctor_id=" +
        std::to_string(doctor_id);
    std::string response = send_get_request(url);

    if (response.empty()) {
        std::cerr << "Empty response from server." << std::endl;
        return;
    }

    try {
        json full_response = json::parse(response);

        if (!full_response.contains("success") || !full_response["success"]) {
            std::cerr << "Error: failed to fetch schedule." << std::endl;
            return;
        }

        json schedule = full_response["schedule"];

        if (!schedule.empty()) {
            for (auto& entry : schedule) {
                // Remove patient_id from output
                entry.erase("patient_id");

                bool is_empty =
                    entry["last_name"].is_null() || entry["last_name"] == "" ||
                    entry["first_name"].is_null() || entry["first_name"] == "" ||
                    entry["patronymic"].is_null() || entry["patronymic"] == "";

                if (is_empty) {
                    entry["status"] = "available";
                } else {
                    entry["status"] = "booked";
                }
            }

            std::cout << "\n=== Doctor Schedule ===\n";
            std::cout << schedule.dump(4) << std::endl;
        } else {
            std::cout << "Schedule is empty.\n";
        }

    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

}  // namespace doctor
