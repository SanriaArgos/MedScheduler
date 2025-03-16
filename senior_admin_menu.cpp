#include "senior_admin_menu.hpp"
#include "utils.hpp"
#include "display_users.hpp"
#include "display_hospitals.hpp"
#include "add_junior_admin.hpp"
#include "add_hospital.hpp"
#include <iostream>
#include <libpq-fe.h>

void senior_admin_menu(database_handler &db, int admin_id) {
    int choice = 0;
    do {
        choice = get_menu_choice("\n=== Senior Administrator Cabinet ===\n1. View Users Table\n2. View Hospitals Table\n3. Add Junior Administrator\n4. Add New Hospital\n5. Exit\nChoose an option: ");
        switch(choice) {
            case 1:
                display_users_table(db);
                break;
            case 2:
                display_hospitals_table(db);
                break;
            case 3:
                add_junior_admin(db);
                break;
            case 4:
                add_hospital(db);
                break;
            case 5:
                std::cout << "Exiting cabinet\n";
                return;
            default:
                std::cout << "Invalid option\n";
                break;
        }
    } while (true);
}
