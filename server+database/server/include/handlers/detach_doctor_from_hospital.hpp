#ifndef DOCTOR_HOSPITAL_MANAGEMENT_HPP_
#define DOCTOR_HOSPITAL_MANAGEMENT_HPP_

#include <nlohmann/json.hpp>
#include "../database.hpp"
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void detach_hospital_from_doctor(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // DOCTOR_HOSPITAL_MANAGEMENT_HPP_