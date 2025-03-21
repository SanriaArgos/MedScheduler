#ifndef ADD_RECORD_SLOT_HPP_
#define ADD_RECORD_SLOT_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция добавления слота записи принимает JSON с полями:
// doctor_id, date, time, hospital_id, cabinet, junior_admin_id.
bool add_record_slot(const nlohmann::json &data);

#endif  // ADD_RECORD_SLOT_HPP_