#ifndef DOCTOR_HOSPITAL_MANAGEMENT_HPP_
#define DOCTOR_HOSPITAL_MANAGEMENT_HPP_

#include "database.hpp"

// Adds the hospital ID (of the junior admin’s hospital) to the doctor's list.
void add_hospital_to_doctor(database_handler &db, int junior_admin_id);

// Removes the hospital ID (of the junior admin’s hospital) from the doctor's list.
void remove_hospital_from_doctor(database_handler &db, int junior_admin_id);

#endif  // DOCTOR_HOSPITAL_MANAGEMENT_HPP_
