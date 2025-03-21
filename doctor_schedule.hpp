#ifndef DOCTOR_SCHEDULE_HPP_
#define DOCTOR_SCHEDULE_HPP_

#include "database.hpp"
#include <string>

// Функция отображения расписания для врача теперь возвращает bool.
// Клиентская логика удалена – для получения расписания используйте JSON-интерфейс.
bool display_doctor_schedule(database_handler &db, int doctor_id);

#endif  // DOCTOR_SCHEDULE_HPP_
