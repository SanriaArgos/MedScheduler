#include "utils.h"
#include <iostream>
#include <string>

namespace {
std::string trim_whitespace(const std::string &str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end   = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}
}

std::string read_line_with_backspace(const std::string &prompt) {
    std::cout << prompt;
    std::string result;
    while (true) {
        int ch = std::cin.get();
        if (ch == '\n' || ch == '\r' || ch == EOF)
            break;
        if (ch == 8 || ch == 127) {
            if (!result.empty()) {
                result.pop_back();
            }
        } else {
            result.push_back(static_cast<char>(ch));
        }
    }
    return result;
}

std::string get_input_backspace(const std::string &prompt) {
    while (true) {
        std::string line = read_line_with_backspace(prompt);
        line = trim_whitespace(line);
        if (line.empty()) {
            std::cout << "Error: Field cannot be empty\n";
            continue;
        }
        return line;
    }
}

std::string get_input(const std::string &prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return trim_whitespace(line);
}

std::string get_validated_input(const std::string &prompt, bool required) {
    while (true) {
        std::string input = get_input(prompt + ": ");
        if (required && input.empty()) {
            std::cout << "Error: Field is required\n";
            continue;
        }
        if (input.find(' ') != std::string::npos) {
            std::cout << "Error: Spaces are not allowed\n";
            continue;
        }
        return input;
    }
}

// Функция is_cyrillic теперь проверяет, что строка содержит только латинские буквы и символ '-'
bool is_latin_or_dash(const std::string &s) {
    for (char c : s) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '-'))) {
            return false;
        }
    }
    return true;
}

bool is_latin_for_mail(const std::string &s) {
    for (char c : s) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '@') || (c=='.') || (c >= '0' && c <= '9'))) {
            return false;
        }
    }
    return true;
}

std::string get_validated_name(const std::string &prompt, bool required) {
    while (true) {
        std::string name = get_validated_input(prompt, required);
        if (!is_latin_or_dash(name)) {
            std::cout << "Error: Use only Latin letters and '-'\n";
            continue;
        }
        return name;
    }
}

bool validate_password_characters(const std::string &password) {
    for (char c : password) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || std::ispunct(static_cast<unsigned char>(c))))
            return false;
    }
    return true;
}

std::string get_validated_password() {
    while (true) {
        std::string password = get_validated_input("Password (min 8 characters, Latin letters, digits, and symbols)", true);
        if (password.size() < 8) {
            std::cout << "Error: Minimum 8 characters\n";
            continue;
        }
        if (!validate_password_characters(password)) {
            std::cout << "Error: Invalid characters\n";
            continue;
        }
        std::string confirm = get_validated_input("Repeat Password", true);
        if (password != confirm) {
            std::cout << "Error: Passwords do not match\n";
            continue;
        }
        return password;
    }
}

bool is_validated_phone(const std::string &s) {
    std::string phone = s;
    if (phone.size() != 12 || phone[0] != '+' || phone[1] != '7') {
        return false;
    }
    for (size_t i = 2; i < phone.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(phone[i]))) {
            return false;
        }
    }
    return true;
}

int get_menu_choice(const std::string &menu) {
    while (true) {
        std::string line = get_input(menu);
        if (line.empty()) {
            std::cout << "Invalid value\n";
            continue;
        }
        try {
            int val = static_cast<int>(std::stoi(line));
            return val;
        } catch (...) {
            std::cout << "Invalid value\n";
        }
    }
}
