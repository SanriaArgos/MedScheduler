#ifndef ATTACH_DOCTOR_TO_HOSPITAL_HPP
#define ATTACH_DOCTOR_TO_HOSPITAL_HPP

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void attach_doctor_to_hospital(
    const json &body,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // ATTACH_DOCTOR_TO_HOSPITAL_HPP
