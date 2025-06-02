#ifndef PATIENT_SCHEDULE_HPP_
#define PATIENT_SCHEDULE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /get_doctor_schedule_for_patient
// Возвращает расписание врача в конкретной больнице с полями:
//   appointment_date,
//   appointment_time,
//   region,
//   settlement_type,
//   settlement_name,
//   street,
//   house,
//   full_name,               — название клиники
//   junior_admin_phone,      — телефон младшего администратора
//   slot_status              — "open" если patient_id IS NULL, иначе "close"
//
// Параметры:
//   doctor_id, hospital_id
void get_doctor_schedule_for_patient(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // PATIENT_SCHEDULE_HPP_
