#include "client_patient.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace patient {

using json = nlohmann::json;

patient_client::patient_client(int user_id) : user_id(user_id) {}

void patient_client::run_menu() {
    int choice = 0;

    while (true) {
        std::cout << "\n=== Patient Menu ===\n";
        std::cout << "1. View doctor's schedule\n";
        std::cout << "2. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                view_doctor_schedule();
                break;
            }

            case 2: {
                std::cout << "Exiting the program.\n";
                return;
            }

            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
}

void patient_client::view_doctor_schedule() {
    std::string url = "http://localhost:8080/doctor_schedule";
    std::string response = send_get_request(url);

    try {
        json schedule = json::parse(response);
        std::cout << "\n=== Doctor's Schedule ===\n";
        for (const auto& entry : schedule) {
            std::cout << "Date: " << entry["appointment_date"] << "\n";
            std::cout << "Time: " << entry["appointment_time"] << "\n";
            std::cout << "Hospital: " << entry["hospital_name"] << "\n";
            std::cout << "Cabinet: " << entry["cabinet_number"] << "\n";
            if (entry["patient_name"].is_null()) {
                std::cout << "Patient: free\n";
            } else {
                std::cout << "Patient: " << entry["patient_name"] << "\n";
            }
            std::cout << "----------------------------\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing schedule: " << e.what() << std::endl;
    }
}

} // namespace patient