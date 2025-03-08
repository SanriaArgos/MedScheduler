#include "auth.hpp"

namespace Auth {

/**
 * @brief Удаляет пробелы и табуляцию с начала и конца строки.
 */
std::string trimWhitespace(const std::string &str) {
  size_t start = str.find_first_not_of(" \t");
  size_t end = str.find_last_not_of(" \t");
  return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/**
 * @brief Проверяет, что номер телефона не пустой и начинается с '+'.
 */
bool validatePhone(const std::string &phone) {
  return !phone.empty() && phone[0] == '+';
}

/**
 * @brief Проверяет, что оба введённых пароля совпадают и не пусты.
 */
bool validatePassword(const std::string &pass1, const std::string &pass2) {
  return !pass1.empty() && (pass1 == pass2);
}

} // namespace Auth
