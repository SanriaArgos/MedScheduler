#include "input_utils.hpp"
#include "auth.hpp"
#include "common.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <locale>
#include <codecvt>
#include <cctype>

std::string get_Input(const std::string &prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return Auth::trim_Whitespace(input);
}

std::string get_Validated_Input(const std::string &prompt, bool required) {
    std::string input;
    while (true) {
        input = get_Input(prompt + " (или 'cancel' для отмены): ");
        if (input == CANCEL_COMMAND)
            return input;
        if (required && input.empty()) {
            std::cout << "Ошибка: поле обязательно для заполнения!\n";
            continue;
        }
        if (input.find(' ') != std::string::npos) {
            std::cout << "Ошибка: пробелы недопустимы!\n";
            continue;
        }
        break;
    }
    return input;
}

bool isCyrillic(const std::string &s) {
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

std::string get_Validated_Name(const std::string &prompt, bool required) {
    while (true) {
        std::string name = get_Validated_Input(prompt, required);
        if (name == CANCEL_COMMAND)
            return name;
        if (!isCyrillic(name)) {
            std::cout << "Ошибка: допускается только кириллица!\n";
            continue;
        }
        return name;
    }
}

std::string get_Validated_Phone() {
    while (true) {
        std::string phone = get_Validated_Input("Телефон (с +, затем ровно 10 цифр, первая 7 или 8):", true);
        if (phone == CANCEL_COMMAND)
            return phone;
        if (!Auth::validate_Phone(phone)) {
            std::cout << "Ошибка: неверный формат телефона! Он должен начинаться с '+' и содержать ровно 10 цифр, первая из которых 7 или 8.\n";
            continue;
        }
        return phone;
    }
}

bool validate_Password_Characters(const std::string &password) {
    for (char c : password) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || std::ispunct(static_cast<unsigned char>(c))))
            return false;
    }
    return true;
}

std::string get_Validated_Password() {
    while (true) {
        std::string password = get_Validated_Input("Пароль (не менее 8 символов, только латинские буквы, цифры и символы):", true);
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
        std::string confirm = get_Validated_Input("Повторите пароль:", true);
        if (confirm == CANCEL_COMMAND)
            return confirm;
        if (password != confirm) {
            std::cout << "Ошибка: пароли не совпадают!\n";
            continue;
        }
        return password;
    }
}

int get_Menu_Choice(const std::string &menu) {
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
