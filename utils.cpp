#include "utils.hpp"
#include <cctype>
#include <codecvt>
#include <iostream>
#include <limits>
#include <locale>
#include <regex>
#include <string>
#include "auth.hpp"

std::string get_input(const std::string &prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return Auth::trim_whitespace(input);
}

std::string get_validated_input(const std::string &prompt, bool required) {
    std::string input;
    while (true) {
        input = get_input(prompt + ": ");
        if (required && input.empty()) {
            std::cout << "Ошибка: поле обязательно\n";
            continue;
        }
        if (input.find(' ') != std::string::npos) {
            std::cout << "Ошибка: не используйте пробелы\n";
            continue;
        }
        break;
    }
    return input;
}

bool is_cyrillic(const std::string &s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(s);
    for (wchar_t wc : ws) {
        if (!((wc >= L'А' && wc <= L'Я') || (wc >= L'а' && wc <= L'я') ||
              wc == L'Ё' || wc == L'ё')) {
            return false;
        }
    }
    return true;
}

std::string get_validated_name(const std::string &prompt, bool required) {
    while (true) {
        std::string name = get_validated_input(prompt, required);
        if (!is_cyrillic(name)) {
            std::cout << "Ошибка: используйте только кириллицу\n";
            continue;
        }
        return name;
    }
}

std::string get_validated_phone() {
    while (true) {
        std::string phone =
            get_validated_input("Телефон (с +, ровно 11 цифр, первая 7)", true);
        if (!Auth::validate_phone(phone)) {
            std::cout << "Ошибка: неверный формат телефона\n";
            continue;
        }
        return phone;
    }
}

bool validate_password_characters(const std::string &password) {
    for (char c : password) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) ||
              std::ispunct(static_cast<unsigned char>(c)))) {
            return false;
        }
    }
    return true;
}

std::string get_validated_password() {
    while (true) {
        std::string password = get_validated_input(
            "Пароль (не менее 8 символов, только латиница, цифры и символы)",
            true
        );
        if (password.size() < 8) {
            std::cout << "Ошибка: не короче 8 символов\n";
            continue;
        }
        if (!validate_password_characters(password)) {
            std::cout << "Ошибка: только латиница, цифры и символы\n";
            continue;
        }
        std::string confirm = get_validated_input("Повторите пароль", true);
        if (password != confirm) {
            std::cout << "Ошибка: пароли не совпадают\n";
            continue;
        }
        return password;
    }
}

int get_menu_choice(const std::string &menu) {
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