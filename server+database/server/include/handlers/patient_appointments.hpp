#ifndef PATIENT_APPOINTMENTS_HPP_
#define PATIENT_APPOINTMENTS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /patient_appointments?patient_id=<int>
void patient_appointments(
    int patient_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // PATIENT_APPOINTMENTS_HPP_
