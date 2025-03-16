#include "user_menu.hpp"
#include "utils.hpp"
#include "patient_schedule.hpp"  // Функция для показа расписания врачей для пациента
#include <iostream>

void user_menu(database_handler &db, int patient_id) {
    int choice = 0;
    do {
        choice = get_menu_choice("\n=== Patient Personal Cabinet ===\n1. View Schedule\n2. Exit\nChoose an option: ");
        switch (choice) {
            case 1:
                // Вызов модуля отображения расписания для пациента
                view_doctor_schedule_for_patient(db);
                break;
            case 2:
                std::cout << "Exiting personal cabinet\n";
                break;
            default:
                std::cout << "Invalid input\n";
                break;
        }
    } while (choice != 2);
}
