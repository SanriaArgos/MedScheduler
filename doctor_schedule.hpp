#ifndef DOCTOR_SCHEDULE_HPP_
#define DOCTOR_SCHEDULE_HPP_

#include "database.hpp"
#include <string>

// отображение расписания в личном кабинете врача
void display_doctor_schedule(database_handler &db, int doctor_id);

#endif  // DOCTOR_SCHEDULE_HPP_
