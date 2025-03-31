#include "client_junior_admin.hpp"
#include "common_for_all.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

namespace junior_admin {

using json = nlohmann::json;

junior_admin_client::junior_admin_client(int admin_id) : admin_id(admin_id) {}


void junior_admin_client::run_menu() {
    int choice = 0;
    while (true) {
    std::cout << "\n=== Junior Admin Menu ===\n";
    std::cout << "1. Add doctor\n";
    std::cout << "2. Add record slot\n";
    std::cout << "3. Attach doctor to hospital\n";
    std::cout << "4. Detach doctor from hospital\n";
    std::cout << "5. View doctors table\n";
    std::cout << "6. View users table\n";
    std::cout << "7. View doctor's schedule\n";
    std::cout << "8. Exit\n";
    std::cout << "Choose an option: ";
    std::cin >> choice;

        switch (choice) {
            case 1: { // Добавление врача
                std::string last_name, first_name, patronymic, phone, education, specialty;
                int experience;

                std::cout << "Enter last name: ";
                std::cin >> last_name;
                std::cout << "Enter first name: ";
                std::cin >> first_name;
                std::cout << "Enter patronymic (optional): ";
                std::cin.ignore(); // Игнорируем остаток строки
                std::getline(std::cin, patronymic);
                std::cout << "Enter phone: ";
                std::cin >> phone;
                std::cout << "Enter education: ";
                std::cin.ignore();
                std::getline(std::cin, education);
                std::cout << "Enter specialty: ";
                std::getline(std::cin, specialty);
                std::cout << "Enter experience (in years): ";
                std::cin >> experience;

                json doctor_data = {
                    {"last_name", last_name},
                    {"first_name", first_name},
                    {"patronymic", patronymic},
                    {"phone", phone},
                    {"education", education},
                    {"specialty", specialty},
                    {"experience", experience}
                };
                add_doctor(doctor_data);
                break;
            }

            case 2: { // Добавление слота записи
                int doctor_id, hospital_id, cabinet;
                std::string date, time;

                std::cout << "Enter doctor ID: ";
                std::cin >> doctor_id;
                std::cout << "Enter date (YYYY-MM-DD): ";
                std::cin >> date;
                std::cout << "Enter time (HH:MM): ";
                std::cin >> time;
                std::cout << "Enter hospital ID: ";
                std::cin >> hospital_id;
                std::cout << "Enter cabinet number: ";
                std::cin >> cabinet;

                json slot_data = {
                    {"doctor_id", doctor_id},
                    {"date", date},
                    {"time", time},
                    {"hospital_id", hospital_id},
                    {"cabinet", cabinet}
                };
                add_record_slot(slot_data);
                break;
            }

            case 3: { // Прикрепить врача к больнице
                int doctor_id, hospital_id;

                std::cout << "Enter doctor ID: ";
                std::cin >> doctor_id;
                std::cout << "Enter hospital ID: ";
                std::cin >> hospital_id;

                json attach_data = {
                    {"doctor_id", doctor_id},
                    {"hospital_id", hospital_id}
                };
                attach_doctor_to_hospital(attach_data);
                break;
            }

            case 4: { // Открепить врача от больницы
                int doctor_id, hospital_id;

                std::cout << "Enter doctor ID: ";
                std::cin >> doctor_id;
                std::cout << "Enter hospital ID: ";
                std::cin >> hospital_id;

                json detach_data = {
                    {"doctor_id", doctor_id},
                    {"hospital_id", hospital_id}
                };
                detach_doctor_from_hospital(detach_data);
                break;
            }

            case 5: { // Просмотреть таблицу врачей
                json doctors = get_doctors_table();
                if (!doctors.empty()) {
                    std::cout << "\n=== Doctors Table ===\n";
                    std::cout << doctors.dump(4) << std::endl; // Выводим JSON в читаемом формате - потом Аюр норм обработает
                } else {
                    std::cout << "Error: Failed to fetch doctors data.\n";
                }
                break;
            }

            case 6: { // Просмотреть таблицу пользователей
                json users = get_users_table();
                if (!users.empty()) {
                    std::cout << "\n=== Users Table ===\n";
                    std::cout << users.dump(4) << std::endl; // Выводим JSON в читаемом формате - потом Аюр норм обработает
                } else {
                    std::cout << "Error: Failed to fetch users data.\n";
                }
                break;
            }

            case 7: { // Просмотреть расписание врача
                int doctor_id;
                std::cout << "Enter doctor ID: ";
                std::cin >> doctor_id;

                json schedule = get_doctor_schedule(doctor_id);
                if (!schedule.empty()) {
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
                } else {
                    std::cout << "Error: Failed to fetch doctor's schedule.\n";
                }
                break;
            }

            case 8: { 
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

void junior_admin_client::add_doctor(const json& data) {
    std::string json_data = data.dump();
    std::string url = "http://localhost:8080/add_doctor";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

void junior_admin_client::add_record_slot(const json& data) {
    std::string json_data = data.dump();
    std::string url = "http://localhost:8080/add_record_slot";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

bool junior_admin_client::check_doctor_exists(int doctor_id) {
    std::string url = "http://localhost:8080/doctors/" + std::to_string(doctor_id);
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

bool junior_admin_client::check_hospital_exists(int hospital_id) {
    std::string url = "http://localhost:8080/hospitals/" + std::to_string(hospital_id);
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

bool junior_admin_client::is_doctor_attached_to_hospital(int doctor_id, int hospital_id) {
    std::string url = "http://localhost:8080/doctors/" + std::to_string(doctor_id) + "/hospitals";
    std::string response = send_get_request(url);

    try {
        json hospital_list = json::parse(response);
        for (const auto& hospital : hospital_list) {
            if (hospital["hospital_id"] == hospital_id) {
                return true; // Врач уже привязан к этой больнице
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking doctor's hospitals: " << e.what() << std::endl;
    }
    return false; // Врач не привязан к этой больнице
}

void junior_admin_client::attach_doctor_to_hospital(const json& data) {
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

    if (is_doctor_attached_to_hospital(doctor_id, hospital_id)) {
        std::cerr << "Error: Doctor with ID " << doctor_id << " is already attached to hospital with ID " << hospital_id << ".\n";
        return;
    }

    std::string json_data = data.dump();
    std::string url = "http://localhost:8080/attach_doctor_to_hospital";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

void junior_admin_client::detach_doctor_from_hospital(const json& data) {
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
    std::string url = "http://localhost:8080/detach_doctor_from_hospital";
    std::string response = send_post_request(url, json_data);
    std::cout << "Server response: " << response << std::endl;
}

json junior_admin_client::get_doctors_table() {
    std::string url = "http://localhost:8080/doctors";
    std::string response = send_get_request(url);

    try {
        json doctors = json::parse(response);
        return doctors;
    } catch (const std::exception& e) {
        std::cerr << "Error fetching doctors table: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_users_table() {
    std::string url = "http://localhost:8080/users";
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception& e) {
        std::cerr << "Error fetching users table: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_doctor_schedule(int doctor_id) {
    std::string check_url = "http://localhost:8080/check_doctor_admin_hospital?doctor_id=" 
                            + std::to_string(doctor_id) + "&admin_id=" + std::to_string(admin_id);
    std::string check_response = send_get_request(check_url);

    try {
        json check_result = json::parse(check_response);
        if (!check_result.value("is_valid", false)) {
            std::cerr << "Error: Doctor and admin are not associated with the same hospital.\n";
            return json();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking hospital association: " << e.what() << std::endl;
        return json();
    }

    std::string schedule_url = "http://localhost:8080/doctor_schedule?doctor_id=" + std::to_string(doctor_id);
    std::string schedule_response = send_get_request(schedule_url);

    try {
        json schedule = json::parse(schedule_response);
        return schedule;
    } catch (const std::exception& e) {
        std::cerr << "Error fetching doctor's schedule: " << e.what() << std::endl;
        return json();
    }
}

} // namespace junior_admin