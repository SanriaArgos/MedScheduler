#ifndef GET_PATIENT_APPOINTMENTS_HPP_
#define GET_PATIENT_APPOINTMENTS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /get_patient_appointments?patient_id=<int>
void get_patient_appointments(
    int patient_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // PATIENT_APPOINTMENTS_HPP_
