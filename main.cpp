#include "users.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include <readline.h>
#include <history.h>

// Читает строку с консоли с использованием readline и проверяет отсутствие пробелов
std::string get_validated_input(const std::string &prompt, bool forbid_spaces = true) {
    char* input_c_str = readline(prompt.c_str());
    std::string input_str = input_c_str ? input_c_str : "";
    free(input_c_str);

    // Проверяем наличие пробелов
    if (forbid_spaces && input_str.find(' ') != std::string::npos) {
        std::cerr << "Ошибка: пробелы недопустимы. Повторите ввод.\n";
        return get_validated_input(prompt, forbid_spaces);
    }

    if (input_str.empty()) {
        std::cerr << "Ошибка: поле не должно быть пустым. Повторите ввод.\n";
        return get_validated_input(prompt, forbid_spaces);
    }

    return input_str;
}

// Проверяет корректность даты рождения в формате DD-MM-YYYY и возвращает строку, если формат верный
std::string get_validated_birth_date(const std::string &prompt) {
    std::string date_str = get_validated_input(prompt, false);

    if (date_str.length() != 10 || date_str[2] != '-' || date_str[5] != '-') {
        std::cerr << "Ошибка: неверный формат даты. Используйте формат DD-MM-YYYY.\n";
        return get_validated_birth_date(prompt);
    }

    // Извлекаем день, месяц и год
    int day, month, year;
    try {
        day = std::stoi(date_str.substr(0, 2));
        month = std::stoi(date_str.substr(3, 2));
        year = std::stoi(date_str.substr(6, 4));
    } catch (...) {
        std::cerr << "Ошибка: неверный формат чисел в дате. Повторите ввод.\n";
        return get_validated_birth_date(prompt);
    }

    if (day < 1 || day > 31) {
        std::cerr << "Ошибка: день должен быть в диапазоне от 1 до 31.\n";
        return get_validated_birth_date(prompt);
    }

    if (month < 1 || month > 12) {
        std::cerr << "Ошибка: месяц должен быть в диапазоне от 1 до 12.\n";
        return get_validated_birth_date(prompt);
    }

    return date_str;
}

// Читает целое число, проверяя, что оно находится в заданном диапазоне
int get_validated_integer(const std::string &prompt, int min_val, int max_val) {
    std::string user_input = get_validated_input(prompt, true);
    int number;
    std::istringstream iss(user_input);

    if (!(iss >> number) || number < min_val || number > max_val) {
        std::cerr << "Ошибка: введите число от " << min_val << " до " << max_val << ".\n";
        return get_validated_integer(prompt, min_val, max_val);
    }

    return number;
}

// Запрашивает ввод данных пользователя и возвращает заполненную структуру User
User get_user_input() {
    std::cout << "\nВнимание: Не используйте пробелы в полях и убедитесь, что номер телефона начинается с '+'\n";
    User new_user;

    new_user.last_name = get_validated_input("Введите Фамилию: ");
    new_user.first_name = get_validated_input("Введите Имя: ");
    new_user.patronymic = get_validated_input("Введите Отчество: ");
    new_user.role = get_validated_input("Введите роль (пациент, врач, администратор): ");
    new_user.password_hash = get_validated_input("Введите хеш пароля: ");
    new_user.birth_date = get_validated_birth_date("Введите дату рождения (DD-MM-YYYY): ");
    new_user.email = get_validated_input("Введите email: ");
    
    while (true) {
        new_user.phone = get_validated_input("Введите номер телефона (начинается с '+'): ");
        if (new_user.phone[0] != '+') {
            std::cerr << "Ошибка: номер телефона должен начинаться с '+'. Повторите ввод.\n";
        } else {
            break;
        }
    }

    return new_user;
}

// Выводит на экран меню операций
void display_menu() {
    std::cout << "\nВыберите операцию:\n";
    std::cout << "1. Создать таблицу Users\n";
    std::cout << "2. Вставить нового пользователя\n";
    std::cout << "3. Обновить данные пользователя\n";
    std::cout << "4. Удалить пользователя\n";
    std::cout << "5. Выход\n";
    std::cout << "Ваш выбор: ";
}

// Главная функция, обеспечивающая взаимодействие с пользователем
int main() {
    UserDatabase database("MedScheduler.db");  // Открываем или создаем базу данных
    int user_choice;
    bool is_running = true;

    while(is_running) {
        display_menu();
        std::string input_line = get_validated_input("", false);
        
        try {
            user_choice = std::stoi(input_line);
        } catch (...) {
            std::cerr << "Неверный выбор. Попробуйте снова.\n";
            continue;
        }

        switch(user_choice) {
            case 1: {
                if(database.createTable()) {
                    std::cout << "Таблица Users успешно создана.\n";
                }
                break;
            }

            case 2: {
                User new_user = get_user_input();
                if(database.insertUser(new_user)) {
                    std::cout << "Пользователь успешно добавлен.\n";
                }
                break;
            }

            case 3: {
                std::string id_input = get_validated_input("Введите id пользователя для обновления: ");
                int user_id;
                try {
                    user_id = std::stoi(id_input);
                } catch (...) {
                    std::cerr << "Неверный id. Попробуйте снова.\n";
                    break;
                }
                if (!database.userExists(user_id)) {
                    std::cerr << "Ошибка: Пользователь с id = " << user_id << " не существует. Обновление отменено.\n";
                    break;
                }
                User updated_user = get_user_input();
                if(database.updateUser(user_id, updated_user)) {
                    std::cout << "Пользователь успешно обновлён.\n";
                }
                break;
            }

            case 4: {
                std::string id_input = get_validated_input("Введите id пользователя для удаления: ");
                int user_id;
                try {
                    user_id = std::stoi(id_input);
                } catch (...) {
                    std::cerr << "Неверный id. Попробуйте снова.\n";
                    break;
                }
                if(database.deleteUser(user_id)) {
                    std::cout << "Пользователь успешно удалён.\n";
                }
                break;
            }

            case 5: {
                is_running = false;
                break;
            }

            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    std::cout << "Работа завершена.\n";
    return 0;
}
