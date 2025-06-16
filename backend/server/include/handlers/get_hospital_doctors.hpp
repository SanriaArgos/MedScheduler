#ifndef GET_HOSPITAL_DOCTORS_HPP_
#define GET_HOSPITAL_DOCTORS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_hospital_doctors(int hospital_id, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // GET_HOSPITAL_DOCTORS_HPP_
