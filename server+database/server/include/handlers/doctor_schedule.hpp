#ifndef DOCTOR_SCHEDULE_HPP_
#define DOCTOR_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция возвращает JSON с личным расписанием врача.
// Ожидается, что входной JSON содержит поле "doctor_id".
json display_doctor_schedule(const json &data);

#endif  // DOCTOR_SCHEDULE_HPP_