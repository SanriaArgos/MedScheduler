#ifndef DOCTOR_CLIENT_HPP
#define DOCTOR_CLIENT_HPP

#include <string>
#include "json.hpp"

using json = nlohmann::json;

void display_doctor_schedule(int doctor_id);

#endif // DOCTOR_CLIENT_HPP