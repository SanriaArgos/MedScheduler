#ifndef JUNIOR_ADMIN_SCHEDULE_HPP_
#define JUNIOR_ADMIN_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Функция возвращает JSON с расписанием врача для младшего администратора.
// Ожидается, что входной JSON содержит поля "junior_admin_id" и "doctor_id".
json junior_admin_schedule(const json &data);

#endif  // JUNIOR_ADMIN_SCHEDULE_HPP_