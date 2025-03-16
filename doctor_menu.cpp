#include "doctor_menu.hpp"
#include "doctor_schedule.hpp" 
#include "utils.hpp"
#include <iostream>
#include <string>

void doctor_menu(database_handler &db, int doctor_id) {
    int choice = 0;
    do {
        choice = get_menu_choice("\n=== Doctor's Personal Cabinet ===\n1. View Schedule\n2. Exit\nChoose an option: ");
        switch (choice) {
            case 1:
                display_doctor_schedule(db, doctor_id);
                break;
            case 2:
                std::cout << "Exiting doctor's cabinet\n";
                return;
            default:
                std::cout << "Invalid input\n";
                break;
        }
    } while (true);
}