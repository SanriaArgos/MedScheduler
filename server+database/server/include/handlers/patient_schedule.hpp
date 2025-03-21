#ifndef PATIENT_SCHEDULE_HPP_
#define PATIENT_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция для просмотра расписания врача для пациента возвращает JSON.
// Ожидается, что входной JSON содержит поля: region, settlement_type, settlement_name, specialty, hospital_id и doctor_id.
json view_doctor_schedule_for_patient(const json &data);

#endif  // PATIENT_SCHEDULE_HPP_