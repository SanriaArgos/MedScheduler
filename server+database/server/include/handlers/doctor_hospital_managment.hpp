#ifndef DOCTOR_HOSPITAL_MANAGEMENT_HPP_
#define DOCTOR_HOSPITAL_MANAGEMENT_HPP_

#include <nlohmann/json.hpp>
#include "../database.hpp"

using json = nlohmann::json;

// Функция добавления больницы в список врача принимает JSON с полями: doctor_id, hospital_id, junior_admin_id.
bool add_hospital_to_doctor(const nlohmann::json &data);

// Функция удаления больницы из списка врача принимает JSON с полями: doctor_id, hospital_id, junior_admin_id.
bool remove_hospital_from_doctor(const nlohmann::json &data);

#endif  // DOCTOR_HOSPITAL_MANAGEMENT_HPP_