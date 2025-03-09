#include "auth.hpp"
#include <cctype>

namespace Auth {

/**
 * @brief Удаляет пробелы и табуляцию с начала и конца строки.
 */
std::string trim_Whitespace(const std::string &str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/**
 * @brief Проверяет, что номер телефона не пустой, начинается с '+', и после '+' ровно 10 цифр, первая из которых 7 или 8.
 */
bool validate_Phone(const std::string &phone) {
    if (phone.empty() || phone[0] != '+') {
        return false;
    }
    std::string digits = phone.substr(1);
    if (digits.size() != 10) {
        return false;
    }
    if (digits[0] != '7' && digits[0] != '8') {
        return false;
    }
    for (char c : digits) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

} // namespace Auth
