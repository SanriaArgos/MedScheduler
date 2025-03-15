#include "senior_admin_menu.hpp"
#include "utils.hpp"
#include "display_users.hpp"
#include "display_hospitals.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>

void add_junior_admin(database_handler &db) {
    std::cout << "\n=== Добавление младшего администратора ===\n";
    std::string last_name = get_validated_name("фамилия", true);
    std::string first_name = get_validated_name("имя", true);
    std::string patronymic = get_input("отчество (необязательно): ");
    if (!patronymic.empty()) {
        while (!is_cyrillic(patronymic)) {
            std::cout << "Ошибка: используйте только кириллицу\n";
            patronymic = get_input("отчество:");
        }
    }
    std::string phone = get_validated_phone();
    std::string default_password = "0987654321";
    if (!db.register_user(last_name, first_name, patronymic, phone, default_password)) {
        std::cout << "Ошибка при добавлении младшего администратора\n";
        return;
    }
    std::string query = "UPDATE users SET user_type = 'младший администратор' WHERE phone = '" + phone + "'";
    PGresult *res = PQexec(db.get_connection(), query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Ошибка обновления типа пользователя: " << PQerrorMessage(db.get_connection()) << "\n";
    }
    PQclear(res);
    std::cout << "Младший администратор добавлен\n";
}

void add_hospital(database_handler &db) {
    std::cout << "\n=== Добавление больницы ===\n";
    auto get_validated_cyrillic = [&](const std::string &prompt) -> std::string {
        return get_validated_input(prompt, true);
    };
    auto get_validated_digits = [&](const std::string &prompt) -> std::string {
        while (true) {
            std::string input = get_validated_input(prompt, true);
            if (std::all_of(input.begin(), input.end(), ::isdigit))
                return input;
            else
                std::cout << "Ошибка: только цифры\n";
        }
    };

    std::string region = get_validated_cyrillic("Область России");
    std::string settlement_type = get_validated_cyrillic("Тип населенного пункта");
    std::string settlement_name = get_validated_cyrillic("Название населенного пункта");
    std::string street = get_validated_cyrillic("Улица");
    std::string house = get_validated_digits("Дом");
    std::string full_name = get_validated_cyrillic("Полное наименование больницы");

    int admin_id;
    while (true) {
        std::string admin_id_str = get_validated_input("ID младшего администратора", true);
        try {
            admin_id = std::stoi(admin_id_str);
        } catch (...) {
            std::cout << "Ошибка: введите числовое значение\n";
            continue;
        }
        std::string query_check = "SELECT user_type FROM users WHERE id = " + std::to_string(admin_id);
        PGresult *res = PQexec(db.get_connection(), query_check.c_str());
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
            std::string user_type = PQgetvalue(res, 0, 0);
            PQclear(res);
            if (user_type != "младший администратор") {
                std::cout << "Ошибка: указанный ID не принадлежит младшему администратору\n";
                continue;
            }
            std::string query_admin = "SELECT 1 FROM hospitals WHERE administrator_id = " + std::to_string(admin_id);
            PGresult *res_admin = PQexec(db.get_connection(), query_admin.c_str());
            if (PQresultStatus(res_admin) == PGRES_TUPLES_OK && PQntuples(res_admin) > 0) {
                std::cout << "Ошибка: данный младший администратор уже привязан к больнице\n";
                PQclear(res_admin);
                continue;
            }
            PQclear(res_admin);
            break;
        } else {
            if (res) PQclear(res);
            std::cout << "Ошибка: не найден младший администратор с таким ID\n";
        }
    }
    std::string query_exist = "SELECT 1 FROM hospitals WHERE region = '" + region +
                              "' AND settlement_type = '" + settlement_type +
                              "' AND settlement_name = '" + settlement_name +
                              "' AND street = '" + street +
                              "' AND house = '" + house + "'";
    PGresult *res_exist = PQexec(db.get_connection(), query_exist.c_str());
    if (PQresultStatus(res_exist) == PGRES_TUPLES_OK && PQntuples(res_exist) > 0) {
        std::cout << "Ошибка: этот адрес уже занят\n";
        PQclear(res_exist);
        return;
    }
    PQclear(res_exist);
    std::stringstream ss;
    ss << "INSERT INTO hospitals (region, settlement_type, settlement_name, street, house, full_name, administrator_id) VALUES ('"
       << region << "', '" << settlement_type << "', '" << settlement_name << "', '" << street << "', '"
       << house << "', '" << full_name << "', " << admin_id << ")";
    PGresult *res_ins = PQexec(db.get_connection(), ss.str().c_str());
    if (PQresultStatus(res_ins) == PGRES_COMMAND_OK) {
        std::cout << "Больница добавлена\n";
    } else {
        std::cout << "Ошибка при добавлении больницы\n";
    }
    PQclear(res_ins);
}

void senior_admin_menu(database_handler &db, int admin_id) {
    int choice = 0;
    do {
        choice = get_menu_choice(
            "\n=== Личный кабинет старшего администратора ===\n"
            "1. Посмотреть таблицу users\n"
            "2. Посмотреть таблицу больниц\n"
            "3. Добавить младшего администратора\n"
            "4. Добавить новую больницу\n"
            "5. Выход\nВыберите действие: "
        );
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
                std::cout << "Выход из аккаунта\n";
                return;
            default:
                std::cout << "Некорректное значение\n";
                break;
        }
    } while (true);
}
