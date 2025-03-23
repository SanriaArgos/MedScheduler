#ifndef DOCTOR_SCHEDULE_HPP_
#define DOCTOR_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция возвращает JSON с личным расписанием врача.
// Ожидается, что входной JSON содержит поле "doctor_id".
void display_doctor_schedule(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // DOCTOR_SCHEDULE_HPP_