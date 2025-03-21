#ifndef ADD_RECORD_SLOT_HPP_
#define ADD_RECORD_SLOT_HPP_

#include "database.hpp"
#include <string>

// Функция для добавления слота записи (приёма).
// Параметры: идентификатор врача, дата, время, идентификатор больницы, номер кабинета и ID младшего администратора.
bool add_record_slot(int doctor_id,
                     const std::string &date,
                     const std::string &time,
                     int hospital_id,
                     int cabinet,
                     int junior_admin_id);

#endif  // ADD_RECORD_SLOT_HPP_
