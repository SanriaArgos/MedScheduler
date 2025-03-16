#include "junior_admin_menu.hpp"
#include "utils.hpp"
#include "display_users.hpp"
#include "display_doctors.hpp"
#include "doctor_hospital_management.hpp"  
#include "add_record_slot.hpp"
#include "junior_admin_schedule.hpp"
#include "add_doctor.hpp"
#include <iostream>

void junior_admin_menu(database_handler &db, int admin_id) {
    int choice = 0;
    do {
        choice = get_menu_choice("\n=== Junior Administrator Cabinet ===\n"
                                 "1. View Users Table\n"
                                 "2. View Doctors Table\n"
                                 "3. Add Doctor\n"
                                 "4. Add Hospital ID to Doctor\n"
                                 "5. Remove Hospital ID from Doctor\n"
                                 "6. Add Appointment Slot\n"
                                 "7. View Doctor Schedule\n"
                                 "8. Exit\n"
                                 "Choose an option: ");
        switch(choice) {
            case 1:
                display_users_table(db);
                break;
            case 2:
                display_doctors_table(db);
                break;
            case 3:
                add_doctor(db, admin_id);
                break;
            case 4:
                add_hospital_to_doctor(db, admin_id);
                break;
            case 5:
                remove_hospital_from_doctor(db, admin_id);
                break;
            case 6:
                add_record_slot(db, admin_id);
                break;
            case 7:
                junior_admin_schedule(db, admin_id);
                break;
            case 8:
                std::cout << "Exiting cabinet\n";
                return;
            default:
                std::cout << "Invalid option\n";
                break;
        }
    } while (true);
}