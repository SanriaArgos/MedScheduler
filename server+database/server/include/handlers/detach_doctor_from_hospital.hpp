#ifndef DETACH_DOCTOR_FROM_HOSPITAL_HPP_
#define DETACH_DOCTOR_FROM_HOSPITAL_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void detach_doctor_from_hospital(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // DETACH_DOCTOR_FROM_HOSPITAL_HPP_