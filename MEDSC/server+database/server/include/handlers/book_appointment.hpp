#ifndef BOOK_APPOINTMENT_HPP_
#define BOOK_APPOINTMENT_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /book_appointment
// JSON { doctor_id, hospital_id, appointment_date, appointment_time, patient_id
// }
void book_appointment(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // BOOK_APPOINTMENT_HPP_
