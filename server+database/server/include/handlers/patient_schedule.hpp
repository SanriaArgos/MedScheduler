#ifndef PATIENT_SCHEDULE_HPP_
#define PATIENT_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция для просмотра расписания врача для пациента возвращает JSON.
// Ожидается, что входной JSON содержит поля: region, settlement_type, settlement_name, specialty, hospital_id и doctor_id.
void view_doctor_schedule_for_patient(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // PATIENT_SCHEDULE_HPP_