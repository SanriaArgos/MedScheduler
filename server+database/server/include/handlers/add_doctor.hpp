#ifndef ADD_DOCTOR_HPP_
#define ADD_DOCTOR_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция добавления врача принимает JSON с полями:
// last_name, first_name, [patronymic], phone, education, specialty, experience.
bool add_doctor(const nlohmann::json &data);

#endif  // ADD_DOCTOR_HPP_