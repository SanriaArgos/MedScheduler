#ifndef PATIENT_SCHEDULE_HPP_
#define PATIENT_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция для просмотра расписания врача для пациента возвращает JSON.
// Ожидается, что передаются два параметра: doctor_id и hospital_id.
void get_doctor_schedule_for_patient(
    int doctor_id,
    int hospital_id,
    http::response<http::string_body>& res,
    database_handler& db_handler
);

#endif  // PATIENT_SCHEDULE_HPP_
