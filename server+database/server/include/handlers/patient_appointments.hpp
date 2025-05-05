#ifndef PATIENT_APPOINTMENTS_HPP_
#define PATIENT_APPOINTMENTS_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /patient_appointments?patient_id=<int>
void patient_appointments(
    int patient_id,
    http::response<http::string_body>& res,
    database_handler& db_handler
);

#endif  // PATIENT_APPOINTMENTS_HPP_
