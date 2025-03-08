#ifndef AUTH_HPP_
#define AUTH_HPP_

#include "database.hpp"
#include <string>

/**
 * @brief Пространство имён Auth содержит функции для валидации и обработки входных данных.
 */
namespace Auth {
    /**
     * @brief Удаляет пробельные символы с начала и конца строки.
     * @param str Входная строка.
     * @return Обрезанная строка без начальных и конечных пробелов.
     */
    std::string trimWhitespace(const std::string &str);

    /**
     * @brief Проверяет, что номер телефона не пуст и начинается с '+'.
     * @param phone Номер телефона.
     * @return true, если формат корректен.
     */
    bool validatePhone(const std::string &phone);

    /**
     * @brief Проверяет, что два пароля совпадают и не пусты.
     * @param pass1 Первый пароль.
     * @param pass2 Второй пароль.
     * @return true, если пароли совпадают и не пусты.
     */
    bool validatePassword(const std::string &pass1, const std::string &pass2);
}

#endif // AUTH_HPP_
