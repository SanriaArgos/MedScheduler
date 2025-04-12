
#include "client_patient.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace patient {

using json = nlohmann::json;

patient_client::patient_client(int user_id) : user_id(user_id) {
}

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
    // Ввод данных от пользователя
    std::string region, settlement_type, settlement_name, specialty;
    int hospital_id, doctor_id;

    std::cout << "Enter region: ";
    std::cin >> region;
    std::cout << "Enter settlement type: ";
    std::cin >> settlement_type;
    std::cout << "Enter settlement name: ";
    std::cin >> settlement_name;
    std::cout << "Enter specialty: ";
    std::cin >> specialty;
    std::cout << "Enter hospital ID: ";
    std::cin >> hospital_id;
    std::cout << "Enter doctor ID: ";
    std::cin >> doctor_id;

    // Создаём JSON с данными
    json request_data = {
        {"region", region},
        {"settlement_type", settlement_type},
        {"settlement_name", settlement_name},
        {"specialty", specialty},
        {"hospital_id", hospital_id},
        {"doctor_id", doctor_id}};

    // Отправляем POST-запрос с JSON
    std::string response = send_post_request(
        "http://localhost:8080/view_doctor_schedule_for_patient", request_data
    );

    if (response.empty()) {
        std::cerr << "Error: Empty response from server\n";
        return;
    }

    try {
        json schedule = json::parse(response);
        if (!schedule["success"]) {
            std::cerr << "Error: " << schedule["error"] << std::endl;
            return;
        }

        std::cout << "\n=== Doctor's Schedule ===\n";
        for (const auto &entry : schedule["schedule"]) {
            std::cout << "Date: " << entry["appointment_date"] << "\n";
            std::cout << "Time: " << entry["appointment_time"] << "\n";
            std::cout << "Cabinet: " << entry["cabinet_number"] << "\n";
            std::cout << "Patient ID: " << entry["patient_id"] << "\n";
            std::cout << "----------------------------\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing schedule: " << e.what() << std::endl;
    }
}

}  // namespace patient