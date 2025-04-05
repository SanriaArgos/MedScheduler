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

// try {
//     json result = json::parse(response);
//     const auto& users = result["users"];
//     std::cout << "\n=== Users Table ===\n";
//     for (const auto& user : users) {

void doctor_client::display_schedule() {
    std::string url =
        "http://localhost:8080/display_doctor_schedule?doctor_id=" +
        std::to_string(doctor_id);
    std::string response = send_get_request(url);

    if (response.empty()) {
        std::cerr << "Получен пустой ответ от сервера." << std::endl;
        return;
    }

    try {
        json full_response = json::parse(response);
        if (!full_response["success"]) {
            std::cerr << "Ошибка: не удалось получить расписание" << std::endl;
            return;
        }

        json schedule = full_response["schedule"];

        std::cout << "\n=== Расписание доктора ===\n";
        std::string current_date;
        for (const auto &entry : schedule) {
            std::string date = entry["appointment_date"];
            std::string time = entry["appointment_time"];
            std::string hospital = entry["full_name"];
            std::string cabinet = entry["cabinet_number"];
            std::string last_name = entry["last_name"];
            std::string first_name = entry["first_name"];
            std::string patronymic = entry["patronymic"];
            std::string patient_phone = entry["phone"];

            if (date != current_date) {  // Выводим по дате
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }

            std::cout << time << ": " << hospital << ", кабинет " << cabinet;
            if (last_name.empty() && first_name.empty() && patronymic.empty()) {
                std::cout << ", свободно";
            } else {
                std::cout << ", " << last_name << " " << first_name << " "
                          << patronymic << ", " << patient_phone;
            }
            std::cout << "\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при парсинге JSON: " << e.what() << std::endl;
    }
}

}  // namespace doctor