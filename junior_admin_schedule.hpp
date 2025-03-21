#ifndef JUNIOR_ADMIN_SCHEDULE_HPP_
#define JUNIOR_ADMIN_SCHEDULE_HPP_

#include "database.hpp"
#include <string>

// Функция расписания для младшего администратора теперь возвращает bool.
// Клиентская (интерактивная) логика удалена – используйте JSON-интерфейс для получения расписания.
bool junior_admin_schedule(database_handler &db, int junior_admin_id);

#endif  // JUNIOR_ADMIN_SCHEDULE_HPP_
