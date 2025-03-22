#include "client_doctor.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace doctor {

using json = nlohmann::json;

doctor_client::doctor_client(int doctor_id) : doctor_id(doctor_id) {}

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
    std::string url = "http://localhost:8080/doctor_schedule?doctor_id=" + std::to_string(doctor_id);
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

            if (date != current_date) { //пока что выводим по дате: то есть сначала все события для одной даты, потом для другой
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

} // namespace doctor