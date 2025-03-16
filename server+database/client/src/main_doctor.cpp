#include "doctor_client.hpp"
#include <iostream>

void display_menu() {
    std::cout << "\n=== Doctor's Menu ===\n";
    std::cout << "1. View schedule\n";
    std::cout << "2. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    int doctor_id = 0;
    int choice = 0;

    std::cin >> doctor_id;

    while (true) {
        display_menu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                display_doctor_schedule(doctor_id);
                break;
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