#include <iostream>
#include "client_doctor.hpp"
#include "client_junior_admin.hpp"
#include "client_senior_admin.hpp"
#include "client_patient.hpp"
#include "client_auth.hpp"

void display_main_menu() {
    std::cout << "\n=== Main Menu ===\n";
    std::cout << "1. Login\n";
    std::cout << "2. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    int choice = 0;

    while (true) {
        display_main_menu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string phone, password;
                std::cout << "Enter phone: ";
                std::cin >> phone;
                std::cout << "Enter password: ";
                std::cin >> password;

                auth::user_info user = auth::login(phone, password);
                if (user.id != -1) {
                    std::cout << "Login successful! User type: " << user.user_type << "\n";

                    if (user.user_type == "doctor") {
                        doctor::doctor_client doctor(user.id);
                        doctor.run_menu();
                    } else if (user.user_type == "junior_admin") {
                        junior_admin::junior_admin_client admin(user.id);
                        admin.run_menu();
                    } else if (user.user_type == "senior_admin") {
                        senior_admin::senior_admin_client senior_admin(user.id);
                        senior_admin.run_menu();
                    } else if (user.user_type == "patient") {
                        patient::patient_client patient;
                        patient.run_menu();
                    } else {
                        std::cerr << "Unknown user type.\n";
                    }
                } else {
                    std::cerr << "Login failed. Please try again.\n";
                }
                break; //exit from switch
            }

            case 2: {
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