#include "auth.hpp"
#include "database.hpp"
#include "registration.hpp"
#include "login.hpp"
#include "user_menu.hpp"
#include "utils.hpp"
#include "handlers.hpp"
#include "system_init.hpp"
#include "senior_admin_menu.hpp"
#include "junior_admin_menu.hpp"
#include "doctor_menu.hpp"
#include "doctor_schedule.hpp"
#include "add_doctor.hpp"
#include <iostream>
#include <string>
#include <limits>

int main() {
    // System initialization: create Meduser and database Medscheduler if not exists.
    const std::string superuser_connect_info = "dbname=postgres user=postgres password=123 host=localhost port=5432";
    if (!initialize_system(superuser_connect_info)) {
        std::cerr << "System initialization error\n";
        return 1;
    }

    // Connect to Medscheduler as Meduser.
    const std::string connect_information = "dbname=medscheduler user=meduser password=3671920119 host=localhost port=5432";
    database_handler db(connect_information);
    if (!db.connect()) {
        std::cerr << "Database connection error\n";
        return 1;
    }
    if (!db.initialize_database()) {
        std::cerr << "Database initialization error\n";
        return 1;
    }

    int choice = 0;
    do {
        choice = get_menu_choice("\n1. Registration\n2. Login\n3. Exit\nChoose an option: ");
        switch (choice) {
            case 1:
                registration(db);
                break;
            case 2: {
                std::string login_result = login(db);
                if (login_result.empty()) {
                    std::cout << "Invalid credentials\n";
                    break;
                }
                if (login_result.rfind("patient:", 0) == 0) {
                    int patient_id = std::stoi(login_result.substr(8));
                    user_menu(db, patient_id);
                } else if (login_result.rfind("senior:", 0) == 0) {
                    int admin_id = std::stoi(login_result.substr(7));
                    senior_admin_menu(db, admin_id);
                } else if (login_result.rfind("junior:", 0) == 0) {
                    int admin_id = std::stoi(login_result.substr(7));
                    junior_admin_menu(db, admin_id);
                } else if (login_result.rfind("doctor:", 0) == 0) {
                    int doctor_id = std::stoi(login_result.substr(7));
                    doctor_menu(db, doctor_id);
                } else {
                    std::cout << "Login successful\n";
                }
            } break;
            case 3:
                std::cout << "Exiting application\n";
                break;
            default:
                std::cout << "Invalid input\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
