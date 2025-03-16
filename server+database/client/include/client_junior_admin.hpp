#ifndef CLIENT_JUNIOR_ADMIN_HPP
#define CLIENT_JUNIOR_ADMIN_HPP

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void add_doctor(const json& data);

void add_record_slot(const json& data);

void attach_doctor_to_hospital(const json& data);

void detach_doctor_from_hospital(const json& data);

bool check_doctor_exists(int doctor_id);

bool check_hospital_exists(int hospital_id);

json get_doctors_table(); //вернем джейсон - интерфейс потом выведет уже

json get_users_table(); //вернем джейсон - интерфейс потом выведет уже

json get_doctor_schedule(int doctor_id); //вернем джейсон - интерфейс потом выведет уже

#endif // CLIENT_JUNIOR_ADMIN_HPP