#include "auth.hpp"
#include "database.hpp"
#include "registration.hpp"
#include "login.hpp"
#include "user_menu.hpp"
#include "utils.hpp"
#include "handlers.hpp"
#include "system_init.hpp"
#include "senior_admin_menu.hpp"
#include <iostream>
#include <string>
#include <limits>

int main() {
    const std::string superuser_connect_info = "dbname=postgres user=postgres password=123 host=localhost port=5432";
    if (!initialize_system(superuser_connect_info)) {
        std::cerr << "Ошибка инициализации системы\n";
        return 1;
    }

    const std::string connect_information = "dbname=medscheduler user=meduser password=3671920119 host=localhost port=5432";
    database_handler db(connect_information);
    if (!db.connect()) {
        std::cerr << "Ошибка подключения к БД\n";
        return 1;
    }
    if (!db.initialize_database()) {
        std::cerr << "Ошибка инициализации БД\n";
        return 1;
    }

    int choice = 0;
    do {
        choice = get_menu_choice("\n1. Регистрация\n2. Вход в личный кабинет\n3. Выход из приложения\nВыберите действие: ");
        switch (choice) {
            case 1:
                registration(db);
                break;
            case 2: {
                std::string login_result = login(db);
                if (login_result.empty()) {
                    std::cout << "Некорректные данные\n";
                    break;
                }
                if (login_result.rfind("patient:", 0) == 0) {
                    int patient_id = std::stoi(login_result.substr(8));
                    user_menu(db, patient_id);
                } else if (login_result.rfind("senior:", 0) == 0) {
                    int admin_id = std::stoi(login_result.substr(7));
                    senior_admin_menu(db, admin_id);
                } else {
                    std::cout << "Вход выполнен\n";
                }
            } break;
            case 3:
                std::cout << "Выход из приложения\n";
                break;
            default:
                std::cout << "Некорректное значение\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
