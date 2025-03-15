#include "user_menu.hpp"
#include <iostream>
#include "utils.hpp"

void user_menu(database_handler &db, int patient_id) {
    int choice = 0;
    do {
        choice = get_menu_choice(
            "\n=== Личный кабинет ===\n1. Просмотр записей\n2. Выход\nВыберите "
            "действие: "
        );
        switch (choice) {
            case 1: {
                std::cout << "\n=== Просмотр записей ===\n";
                std::string records = db.get_patient_records(patient_id);
                if (records.empty()) {
                    std::cout << "У вас пока нет записей\n";
                } else {
                    std::cout << records << "\n";
                }
            } break;
            case 2:
                std::cout << "Вы вышли из личного кабинета\n";
                break;
            default:
                std::cout << "Некорректное значение\n";
                break;
        }
    } while (choice != 2);
}