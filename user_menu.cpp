#include "user_menu.hpp"
#include "utils.hpp"
#include <iostream>

void user_menu(database_handler &db, int patient_id) {
    int choice = 0;
    do {
        choice = get_menu_choice("\n=== User Cabinet ===\n1. View Schedule\n2. Exit\nChoose an option: ");
        switch (choice) {
            case 1:
                std::cout << "\n=== View Schedule ===\n";
                std::cout << "Temporarily unavailable\n";
                break;
            case 2:
                std::cout << "You have exited the cabinet\n";
                break;
            default:
                std::cout << "Invalid option\n";
                break;
        }
    } while (choice != 2);
}
