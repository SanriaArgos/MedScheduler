#ifndef CLIENT_PATIENT_HPP
#define CLIENT_PATIENT_HPP

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void registration();

std::string login();

void view_doctor_schedule();

#endif // CLIENT_PATIENT_HPP