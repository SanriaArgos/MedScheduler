#ifndef PATIENT_SCHEDULE_HPP_
#define PATIENT_SCHEDULE_HPP_

#include "database.hpp"
#include <string>

// Показывает расписание врача для пациента через JSON-интерфейс.
// Клиентская интерактивная логика удалена; функция возвращает false (заглушка).
bool view_doctor_schedule_for_patient(database_handler &db);

#endif  // PATIENT_SCHEDULE_HPP_
