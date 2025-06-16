#ifndef DOCTOR_SCHEDULE_HPP_
#define DOCTOR_SCHEDULE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция возвращает JSON с личным расписанием врача.
// Ожидается, что входной JSON содержит поле "doctor_id".
void get_doctor_schedule(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // DOCTOR_SCHEDULE_HPP_