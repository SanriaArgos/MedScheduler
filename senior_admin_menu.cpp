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
        return get_validated_input(prompt, true); // Предполагается, что get_validated_input уже гарантирует отсутствие пробелов
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

void delete_user(database_handler &db, int senior_admin_id) {
    std::cout << "\n=== Удаление пользователя ===\n";
    std::string input_id = get_validated_input("Введите ID пользователя", true);
    int user_id = std::stoi(input_id);
    if (user_id == senior_admin_id) {
        std::cout << "Удалить старшего администратора нельзя\n";
        return;
    }
    std::string answer = get_validated_input("Уверены, что хотите удалить пользователя? (да/нет)", true);
    if (answer != "да") {
        std::cout << "Отмена удаления\n";
        return;
    }
    std::string query_type = "SELECT user_type FROM users WHERE id = " + std::to_string(user_id);
    PGresult *res = PQexec(db.get_connection(), query_type.c_str());
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        std::string user_type = PQgetvalue(res, 0, 0);
        PQclear(res);
        if (user_type == "младший администратор") {
            std::string del_hosp = "DELETE FROM hospitals WHERE administrator_id = " + std::to_string(user_id);
            PQexec(db.get_connection(), del_hosp.c_str());
        }
    } else {
        if (res) PQclear(res);
    }
    std::string query_del = "DELETE FROM users WHERE id = " + std::to_string(user_id);
    PGresult *res_del = PQexec(db.get_connection(), query_del.c_str());
    if (PQresultStatus(res_del) == PGRES_COMMAND_OK) {
        std::cout << "Пользователь удалён\n";
    } else {
        std::cout << "Ошибка при удалении пользователя\n";
    }
    PQclear(res_del);
}

// Функция удаления больницы удалена согласно требованию.

void senior_admin_menu(database_handler &db, int admin_id) {
    int choice = 0;
    do {
        choice = get_menu_choice(
            "\n=== Личный кабинет старшего администратора ===\n"
            "1. Посмотреть таблицу users\n"
            "2. Посмотреть таблицу hospitals\n"
            "3. Добавить младшего администратора\n"
            "4. Добавить новую больницу\n"
            "5. Удалить пользователя\n"
            "6. Выход\nВыберите действие: "
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
                delete_user(db, admin_id);
                break;
            case 6:
                std::cout << "Выход из аккаунта\n";
                return;
            default:
                std::cout << "Некорректное значение\n";
                break;
        }
    } while (true);
}
