#ifndef GET_DOCTOR_PROFILE_HPP_
#define GET_DOCTOR_PROFILE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_doctor_profile(int doctor_id, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // GET_DOCTOR_PROFILE_HPP_
