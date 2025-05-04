#ifndef PATIENT_SCHEDULE_HPP_
#define PATIENT_SCHEDULE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция для просмотра расписания врача для пациента возвращает JSON.
// Ожидается, что входной JSON содержит поля: region, settlement_type,
// settlement_name, specialty, hospital_id и doctor_id.
void get_doctors_for_patient(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // PATIENT_SCHEDULE_HPP_