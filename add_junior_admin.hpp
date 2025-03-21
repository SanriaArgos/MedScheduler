#ifndef ADD_JUNIOR_ADMIN_HPP_
#define ADD_JUNIOR_ADMIN_HPP_

#include "database.hpp"
#include <string>

// Функция для добавления младшего администратора. Данные передаются в виде параметров.
bool add_junior_admin(const std::string &last_name,
                      const std::string &first_name,
                      const std::string &patronymic,
                      const std::string &phone);

#endif  // ADD_JUNIOR_ADMIN_HPP_
