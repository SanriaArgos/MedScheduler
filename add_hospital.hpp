#ifndef ADD_HOSPITAL_HPP_
#define ADD_HOSPITAL_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция добавления больницы принимает JSON с полями: region, settlement_type, settlement_name, street, house, full_name, admin_id.
bool add_hospital(const nlohmann::json &data);

#endif  // ADD_HOSPITAL_HPP_
