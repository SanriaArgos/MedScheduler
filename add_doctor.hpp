#ifndef ADD_DOCTOR_HPP_
#define ADD_DOCTOR_HPP_

#include "database.hpp"
#include <string>

// Новая функция для добавления врача; данные передаются в виде параметров,
// а доступ к базе осуществляется через глобальный объект.
bool add_doctor(const std::string &last_name,
                const std::string &first_name,
                const std::string &patronymic,
                const std::string &phone,
                const std::string &education,
                const std::string &specialty,
                int experience);

#endif  // ADD_DOCTOR_HPP_
