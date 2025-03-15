#include "user_menu.hpp"
#include "utils.hpp"
#include <iostream>

void user_menu(database_handler &db, int patient_id) {
    int choice = 0;
    do {
        choice = get_menu_choice("\n=== Личный кабинет ===\n1. Просмотр расписания\n2. Выход\nВыберите действие: ");
        switch (choice) {
            case 1:
                std::cout << "\n=== Просмотр расписания ===\n";
                std::cout << "временно недоступно\n";
                break;
            case 2:
                std::cout << "Вы вышли из личного кабинета\n";
                break;
            default:
                std::cout << "Некорректное значение\n";
                break;
        }
    } while (choice != 2);
}