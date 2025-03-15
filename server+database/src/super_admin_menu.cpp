#include "super_admin_menu.hpp"
#include "common.hpp"
#include "handlers.hpp"
#include "input_utils.hpp" // Для вызова функций get_Validated_Input, get_Validated_Name и т.д.
#include <iostream>
#include <limits>
#include <regex>
#include <string>

// Функции валидации для полей больницы
static std::string get_Validated_Cyrillic(const std::string &prompt, bool required) {
    while (true) {
        std::string input = get_Validated_Input(prompt, required);
        if (input == CANCEL_COMMAND)
            return input;
        // Разрешаются кириллица и двойные кавычки
        std::regex pattern("^[А-ЯЁа-яё\"]+$");
        if (std::regex_match(input, pattern))
            return input;
        else
            std::cout << "Ошибка: поле может содержать только кириллицу и двойные кавычки.\n";
    }
}

static std::string get_Validated_Digits(const std::string &prompt, bool required) {
    while (true) {
        std::string input = get_Validated_Input(prompt, required);
        if (input == CANCEL_COMMAND)
            return input;
        if (std::all_of(input.begin(), input.end(), ::isdigit))
            return input;
        else
            std::cout << "Ошибка: поле может содержать только цифры.\n";
    }
}

static int getMenuChoice(const std::string &menu) {
    int choice = 0;
    while (true) {
        std::cout << menu;
        if (std::cin >> choice) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        } else {
            std::cout << "Некорректное значение\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return choice;
}

static void addJuniorAdmin(Database_Handler &db) {
    std::cout << "\n=== Добавление младшего администратора ===\n";
    std::string last_name = get_Validated_Name("Фамилия: ", true);
    if (last_name == CANCEL_COMMAND) return;
    std::string first_name = get_Validated_Name("Имя: ", true);
    if (first_name == CANCEL_COMMAND) return;
    std::string patronymic = get_Input("Отчество (необязательно): ");
    if (patronymic == CANCEL_COMMAND) return;
    if (!patronymic.empty()) {
        while (!isCyrillic(patronymic)) {
            std::cout << "Ошибка: допускается только кириллица!\n";
            patronymic = get_Input("Отчество (или 'cancel'): ");
            if (patronymic == CANCEL_COMMAND) return;
        }
    }
    std::string phone = get_Validated_Phone();
    if (phone == CANCEL_COMMAND) return;
    std::string default_password = "0987654321";
    bool ok = db.register_User(last_name, first_name, patronymic, phone, default_password);
    if (!ok) {
        std::cout << "Ошибка при добавлении младшего администратора!\n";
        return;
    }
    db.setUserTypeByPhone(phone, "младший администратор");
    std::cout << "Младший администратор добавлен!\n";
}

static void removeUser(Database_Handler &db, int main_admin_id) {
    std::cout << "\n=== Удаление пользователя ===\n";
    std::string input_id = get_Validated_Input("Введите ID пользователя:", true);
    if (input_id == CANCEL_COMMAND) return;
    int user_id = 0;
    try {
        user_id = std::stoi(input_id);
    } catch(...) {
        std::cout << "Ошибка: неверный ввод!\n";
        return;
    }
    if (user_id == main_admin_id) {
        std::cout << "Нельзя удалить главного администратора!\n";
        return;
    }
    std::string answer = get_Validated_Input("Уверены, что хотите удалить пользователя? (да/нет):", true);
    if (answer != "да") {
        std::cout << "Отмена удаления.\n";
        return;
    }
    std::string user_type = db.getUserType(user_id);
    if (user_type == "младший администратор") {
        db.deleteHospitalsWhereAdmin(user_id);
    }
    bool ok = db.deleteUser(user_id);
    if (ok)
        std::cout << "Пользователь удалён.\n";
    else
        std::cout << "Ошибка при удалении.\n";
}

static void addHospital(Database_Handler &db) {
    std::cout << "\n=== Добавление новой больницы ===\n";
    std::string region = get_Validated_Cyrillic("Область России:", true);
    if (region == CANCEL_COMMAND) return;
    std::string settlement_type = get_Validated_Cyrillic("Тип населённого пункта:", true);
    if (settlement_type == CANCEL_COMMAND) return;
    std::string settlement_name = get_Validated_Cyrillic("Название населённого пункта:", true);
    if (settlement_name == CANCEL_COMMAND) return;
    std::string street = get_Validated_Cyrillic("Улица:", true);
    if (street == CANCEL_COMMAND) return;
    std::string full_name = get_Validated_Cyrillic("Полное наименование больницы:", true);
    if (full_name == CANCEL_COMMAND) return;
    std::string house = get_Validated_Digits("Дом:", true);
    if (house == CANCEL_COMMAND) return;
    std::string admin_id_str = get_Validated_Digits("ID младшего администратора:", true);
    if (admin_id_str == CANCEL_COMMAND) return;
    int admin_id = 0;
    try {
        admin_id = std::stoi(admin_id_str);
    } catch(...) {
        std::cout << "Ошибка: некорректный ввод ID.\n";
        return;
    }
    if (db.getUserType(admin_id) != "младший администратор") {
        std::cout << "Ошибка: указанный пользователь не является младшим администратором!\n";
        return;
    }
    bool ok = db.insertHospital(region, settlement_type, settlement_name, street, house, full_name, admin_id);
    if (ok)
        std::cout << "Больница добавлена.\n";
    else
        std::cout << "Ошибка: такая больница уже существует.\n";
}

static void removeHospital(Database_Handler &db) {
    std::cout << "\n=== Удаление больницы ===\n";
    std::string hid_str = get_Validated_Input("Введите ID больницы:", true);
    if (hid_str == CANCEL_COMMAND) return;
    int hid = 0;
    try {
        hid = std::stoi(hid_str);
    } catch(...) {
        std::cout << "Ошибка: некорректный ввод!\n";
        return;
    }
    std::string answer = get_Validated_Input("Уверены, что хотите удалить больницу? (да/нет):", true);
    if (answer != "да") {
        std::cout << "Отмена.\n";
        return;
    }
    bool ok = db.deleteHospital(hid);
    if (ok)
        std::cout << "Больница удалена.\n";
    else
        std::cout << "Ошибка при удалении.\n";
}

static void viewUsers(Database_Handler &db) {
    std::cout << "\n=== Таблица пользователей ===\n";
    std::string table = db.selectAllUsers();
    if (table.empty())
        std::cout << "Нет пользователей.\n";
    else
        std::cout << table << std::endl;
}

static void viewHospitals(Database_Handler &db) {
    std::cout << "\n=== Таблица больниц ===\n";
    std::string table = db.selectAllHospitals();
    if (table.empty())
        std::cout << "Нет больниц.\n";
    else
        std::cout << table << std::endl;
}

static void editMainAdminProfile(Database_Handler &db, int admin_id) {
    std::cout << "\n=== Редактирование профиля главного администратора ===\n";
    std::cout << "Для отмены обновления введите 'cancel' на любом этапе.\n";
    std::string new_last = get_Validated_Name("Новая фамилия:", true);
    if (new_last == CANCEL_COMMAND) {
        std::cout << "Отмена.\n";
        return;
    }
    std::string new_first = get_Validated_Name("Новое имя:", true);
    if (new_first == CANCEL_COMMAND) {
        std::cout << "Отмена.\n";
        return;
    }
    std::string new_patronymic = get_Input("Новое отчество (необязательно): ");
    if (new_patronymic == CANCEL_COMMAND) {
        std::cout << "Отмена.\n";
        return;
    }
    if (!new_patronymic.empty()) {
        while (!isCyrillic(new_patronymic)) {
            std::cout << "Ошибка: допускается только кириллица!\n";
            new_patronymic = get_Input("Введите новое отчество (или 'cancel'): ");
            if (new_patronymic == CANCEL_COMMAND)
                return;
        }
    }
    std::string new_phone = get_Validated_Phone();
    if (new_phone == CANCEL_COMMAND) {
        std::cout << "Отмена.\n";
        return;
    }
    if (!db.update_User_Info(admin_id, new_last, new_first, new_patronymic, new_phone)) {
        std::cout << "Ошибка: телефон уже занят?\n";
    } else {
        std::cout << "Информация успешно обновлена!\n";
    }
}

void superAdminMenu(Database_Handler &db, int admin_id) {
    int choice = 0;
    do {
        std::cout << "\n=== Личный кабинет главного администратора ===\n"
                  << "1. Добавить младшего администратора\n"
                  << "2. Удалить пользователя\n"
                  << "3. Добавить новую больницу\n"
                  << "4. Удалить больницу\n"
                  << "5. Посмотреть таблицу пользователей\n"
                  << "6. Посмотреть таблицу больниц\n"
                  << "7. Редактировать личную информацию\n"
                  << "8. Выход\n";
        choice = getMenuChoice("Выберите действие: ");
        switch(choice) {
            case 1:
                addJuniorAdmin(db);
                break;
            case 2:
                removeUser(db, admin_id);
                break;
            case 3:
                addHospital(db);
                break;
            case 4:
                removeHospital(db);
                break;
            case 5:
                viewUsers(db);
                break;
            case 6:
                viewHospitals(db);
                break;
            case 7:
                editMainAdminProfile(db, admin_id);
                break;
            case 8:
                std::cout << "Выход из учётной записи главного администратора.\n";
                return;
            default:
                std::cout << "Неверный выбор!\n";
                break;
        }
    } while (true);
}
