#include "client_junior_admin.hpp"
#include "common_for_all.hpp"
#include <iostream>

void display_menu() {
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
}

int main() {
    int choice = 0;
    int admin_id = 123;
    std::cin >> admin_id;

    while (true) {
        display_menu();
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

                json schedule = get_doctor_schedule(doctor_id, admin_id);
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
                return 0;
            }

            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}