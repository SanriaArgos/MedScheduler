#include "auth.hpp"
#include "database.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <locale>
#include <codecvt>
#include <cctype>

// Команда отмены
const std::string CANCEL_COMMAND = "cancel";

/**
 * @brief Проверяет, что строка состоит только из кириллических символов.
 * @param s Строка в UTF-8.
 * @return true, если все символы являются кириллическими.
 */
bool isCyrillic(const std::string &s) {
    // Конвертируем UTF-8 строку в широкую строку
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(s);
    for (wchar_t wc : ws) {
        if (!((wc >= L'А' && wc <= L'Я') ||
              (wc >= L'а' && wc <= L'я') ||
              wc == L'Ё' || wc == L'ё'))
            return false;
    }
    return true;
}

/**
 * @brief Считывает строку с консоли, выводя приглашение.
 * @param prompt Приглашение для ввода.
 * @return Строка без начальных и конечных пробелов.
 */
std::string get_Input(const std::string &prompt) {
    std::string input{};
    std::cout << prompt;
    std::getline(std::cin, input);
    return Auth::trim_Whitespace(input);
}

/**
 * @brief Считывает строку, которая не должна быть пустой и не содержать пробелов.
 * Если введено "cancel", возвращается эта строка.
 * @param prompt Приглашение для ввода.
 * @param required Если поле обязательно.
 * @return Считанная строка.
 */
std::string get_Validated_Input(const std::string &prompt, bool required = true) {
    std::string input{};
    while (true) {
        input = get_Input(prompt + " (или 'cancel' для отмены): ");
        if (input == CANCEL_COMMAND)
            return input;
        if (required && input.empty()) {
            std::cout << "Ошибка: поле обязательно для заполнения!\n";
            continue;
        }
        if (input.find(' ') != std::string::npos) {
            std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
            continue;
        }
        break;
    }
    return input;
}

/**
 * @brief Считывает имя (фамилию, имя, отчество) и проверяет, что оно содержит только кириллицу.
 * @param prompt Приглашение для ввода.
 * @param required Если поле обязательно.
 * @return Корректное имя, либо CANCEL_COMMAND при отмене.
 */
std::string get_Validated_Name(const std::string &prompt, bool required = true) {
    while (true) {
        std::string name = get_Validated_Input(prompt, required);
        if (name == CANCEL_COMMAND)
            return name;
        if (!isCyrillic(name)) {
            std::cout << "Ошибка: в этом поле допускается только кириллица!\n";
            continue;
        }
        return name;
    }
}

/**
 * @brief Считывает номер телефона с проверкой формата.
 * @return Корректный номер телефона, либо CANCEL_COMMAND при отмене.
 */
std::string get_Validated_Phone() {
    while (true) {
        std::string phone = get_Validated_Input("Телефон (с +, затем ровно 10 цифр, первая 7 или 8)", true);
        if (phone == CANCEL_COMMAND)
            return phone;
        if (!Auth::validate_Phone(phone)) {
            std::cout << "Ошибка: неверный формат телефона! Телефон должен начинаться с '+' и содержать ровно 10 цифр, первая из которых 7 или 8.\n";
            continue;
        }
        return phone;
    }
}

/**
 * @brief Проверяет, что пароль состоит только из латинских букв, цифр и символов.
 * @param password Введенный пароль.
 * @return true, если пароль корректен.
 */
bool validate_Password_Characters(const std::string &password) {
    for (char c : password) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || std::ispunct(static_cast<unsigned char>(c))))
            return false;
    }
    return true;
}

/**
 * @brief Считывает пароль и его подтверждение, проверяя, что пароль не менее 8 символов и содержит только разрешённые символы.
 * @return Корректный пароль, либо CANCEL_COMMAND при отмене.
 */
std::string get_Validated_Password() {
    while (true) {
        std::string password = get_Validated_Input("Пароль (не менее 8 символов, только латинские буквы, цифры и символы)", true);
        if (password == CANCEL_COMMAND)
            return password;
        if (password.size() < 8) {
            std::cout << "Ошибка: пароль должен содержать не менее 8 символов!\n";
            continue;
        }
        if (!validate_Password_Characters(password)) {
            std::cout << "Ошибка: пароль может содержать только латинские буквы, цифры и символы.\n";
            continue;
        }
        std::string confirm = get_Validated_Input("Повторите пароль", true);
        if (confirm == CANCEL_COMMAND)
            return confirm;
        if (password != confirm) {
            std::cout << "Ошибка: пароли не совпадают!\n";
            continue;
        }
        return password;
    }
}

/**
 * @brief Выводит меню и возвращает выбранный пункт.
 * @param menu Строка с меню.
 * @return Выбранный пункт меню.
 */
int get_Menu_Choice(const std::string &menu) {
    int choice{0};
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

/**
 * @brief Функция редактирования профиля. Позволяет пользователю изменить личную информацию.
 * @param db Объект для работы с базой данных.
 * @param patient_id Идентификатор пациента.
 */
void edit_Profile(Database_Handler &db, int patient_id) {
    std::cout << "Для отмены обновления введите 'cancel' на любом этапе.\n";
    const std::string new_last_name = get_Validated_Name("Введите новую фамилию: ");
    if (new_last_name == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    const std::string new_first_name = get_Validated_Name("Введите новое имя: ");
    if (new_first_name == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    std::string new_patronymic = get_Input("Введите новое отчество (необязательно): ");
    if (new_patronymic == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    if (!new_patronymic.empty()) {
        while (!isCyrillic(new_patronymic)) {
            std::cout << "Ошибка: в отчеестве допускается только кириллица.\n";
            new_patronymic = get_Input("Введите новое отчество (без латинских символов и цифр, или 'cancel' для отмены): ");
            if (new_patronymic == CANCEL_COMMAND) {
                std::cout << "Обновление отменено.\n";
                return;
            }
        }
    }
    std::string new_phone = get_Validated_Phone();
    if (new_phone == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    while (!db.update_User_Info(patient_id, new_last_name, new_first_name, new_patronymic, new_phone)) {
        std::cout << "Ошибка: выбранный телефон уже используется другим пользователем. Введите другой номер.\n";
        new_phone = get_Validated_Phone();
        if (new_phone == CANCEL_COMMAND) {
            std::cout << "Обновление отменено.\n";
            return;
        }
    }
    std::cout << "Личная информация успешно обновлена.\n";
}

/**
 * @brief Выводит меню личного кабинета пациента.
 * @param db Объект для работы с базой данных.
 * @param patient_id Идентификатор пациента.
 */
void patient_Menu(Database_Handler &db, int patient_id) {
    int choice = 0;
    do {
        choice = get_Menu_Choice("\n=== Личный кабинет ===\n1. Просмотр записей\n2. Редактировать личную информацию\n3. Выход\nВыберите действие: ");
        switch (choice) {
            case 1: {
                std::cout << "\n=== Просмотр записей ===\n";
                std::string records = db.get_Patient_Records(patient_id);
                if (records.empty()) {
                    std::cout << "У вас пока нет записей\n";
                } else {
                    std::cout << records << std::endl;
                }
            } break;
            case 2:
                edit_Profile(db, patient_id);
                break;
            case 3:
                std::cout << "Выход из личного кабинета.\n";
                break;
            default:
                std::cout << "Неверный выбор!\n";
                break;
        }
    } while (choice != 3);
}

/**
 * @brief Обрабатывает регистрацию пользователя.
 * @param db Объект для работы с базой данных.
 */
void handle_Registration(Database_Handler &db) {
    std::cout << "Для отмены регистрации введите 'cancel' на любом этапе.\n";
    const std::string last_name = get_Validated_Name("Фамилия: ");
    if (last_name == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    const std::string first_name = get_Validated_Name("Имя: ");
    if (first_name == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    std::string patronymic = get_Input("Отчество (необязательно): ");
    if (patronymic == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    if (!patronymic.empty()) {
        while (!isCyrillic(patronymic)) {
            std::cout << "Ошибка: в отчеестве допускается только кириллица!\n";
            patronymic = get_Input("Отчество (без латинских символов и цифр, или 'cancel' для отмены): ");
            if (patronymic == CANCEL_COMMAND) {
                std::cout << "Регистрация отменена.\n";
                return;
            }
        }
    }
    std::string phone = get_Validated_Phone();
    if (phone == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    while (db.user_Exists(phone)) {
        std::cout << "Ошибка: телефон уже зарегистрирован!\n";
        phone = get_Validated_Phone();
        if (phone == CANCEL_COMMAND) {
            std::cout << "Регистрация отменена.\n";
            return;
        }
    }
    const std::string password = get_Validated_Password();
    if (password == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    if (db.register_User(last_name, first_name, patronymic, phone, password))
        std::cout << "Регистрация прошла успешно!\n";
    else
        std::cout << "Ошибка регистрации!\n";
}

/**
 * @brief Обрабатывает вход пользователя.
 * @param db Объект для работы с базой данных.
 * @return Результат входа: строка "patient:<id>" для пациентов, "success" для других,
 *         CANCEL_COMMAND если вход отменен, либо пустая строка при ошибке.
 */
std::string handle_Login(Database_Handler &db) {
    std::cout << "Для отмены входа введите 'cancel' на любом этапе.\n";
    const std::string phone = get_Validated_Phone();
    if (phone == CANCEL_COMMAND) {
        std::cout << "Вход отменен.\n";
        return CANCEL_COMMAND;
    }
    const std::string password = get_Validated_Input("Пароль: ", false);
    if (password == CANCEL_COMMAND) {
        std::cout << "Вход отменен.\n";
        return CANCEL_COMMAND;
    }
    return db.login_User(phone, password);
}

/**
 * @brief Основная функция, реализующая меню регистрации, входа и работы личного кабинета.
 */
int main() {
    const std::string connect_information = "dbname=medscheduler user=meduser password=3671920119 host=localhost port=5432";
    Database_Handler db(connect_information);
    if (!db.connect()) {
        std::cerr << "Ошибка подключения к БД" << std::endl;
        return 1;
    }

    int choice = 0;
    do {
        choice = get_Menu_Choice("\n1. Регистрация\n2. Вход\n3. Выход\nВыберите действие: ");
        switch (choice) {
            case 1:
                handle_Registration(db);
                break;
            case 2: {
                const std::string login_result = handle_Login(db);
                if (login_result == CANCEL_COMMAND) {
                    break;
                }
                if (login_result.empty()) {
                    std::cout << "Неверные данные!\n";
                    break;
                }
                if (login_result.rfind("patient:", 0) == 0) {
                    int patient_id = static_cast<int>(std::stoi(login_result.substr(8)));
                    patient_Menu(db, patient_id);
                } else {
                    std::cout << "Вход выполнен!\n";
                }
            } break;
            case 3:
                std::cout << "Выход из приложения.\n";
                break;
            default:
                std::cout << "Неверный выбор!\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
