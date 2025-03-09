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
     * @return Строка без начальных и конечных пробелов.
     */
    std::string trim_Whitespace(const std::string &str);

    /**
     * @brief Проверяет, что номер телефона не пуст, начинается с '+' и после '+' ровно 10 цифр, первая из которых 7 или 8.
     * @param phone Номер телефона.
     * @return true, если формат корректен.
     */
    bool validate_Phone(const std::string &phone);
}

#endif  // AUTH_HPP_
