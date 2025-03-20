#include "client_patient.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace patient {

using json = nlohmann::json;

void patient_client::registration() {
    std::cout << "\n=== Registration ===\n";
    std::string last_name, first_name, patronymic, phone, password;

    std::cout << "Enter last name: ";
    std::cin >> last_name;
    std::cout << "Enter first name: ";
    std::cin >> first_name;
    std::cout << "Enter patronymic (optional): ";
    std::cin.ignore(); // Игнорируем остаток строки
    std::getline(std::cin, patronymic);
    std::cout << "Enter phone: ";
    std::cin >> phone;
    std::cout << "Enter password: ";
    std::cin >> password;

    json registration_data = {
        {"last_name", last_name},
        {"first_name", first_name},
        {"patronymic", patronymic},
        {"phone", phone},
        {"password", password}
    };

    std::string json_data = registration_data.dump();
    std::string url = "http://localhost:3000/register";
    std::string response = send_post_request(url, json_data);

    std::cout << "Server response: " << response << std::endl;
}

void patient_client::login() {
    std::cout << "\n=== Login ===\n";
    std::string phone, password;

    std::cout << "Enter phone: ";
    std::cin >> phone;
    std::cout << "Enter password: ";
    std::cin >> password;

    json login_data = {
        {"phone", phone},
        {"password", password}
    };

    std::string json_data = login_data.dump();
    std::string url = "http://localhost:3000/login";
    std::string response = send_post_request(url, json_data);

    try {
        json response_data = json::parse(response);
        if (response_data.contains("message")) {
            std::cout << "Login successful!\n";
        } else {
            std::cerr << "Error: " << response_data.value("error", "Login failed") << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing server response: " << e.what() << std::endl;
    }
}

void patient_client::view_doctor_schedule() {
    std::string url = "http://localhost:3000/doctor_schedule";
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

void patient_client::run_menu() {
    int choice = 0;

    while (true) {
        std::cout << "\n=== Patient Menu ===\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. View doctor's schedule\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                registration();
                break;
            }

            case 2: {
                login();
                break;
            }

            case 3: {
                view_doctor_schedule();
                break;
            }

            case 4: {
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

} // namespace patient