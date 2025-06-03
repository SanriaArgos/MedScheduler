#ifndef GET_DOCTOR_AVERAGE_RATINGS_HPP_
#define GET_DOCTOR_AVERAGE_RATINGS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /filters/doctor_average_ratings
void get_doctor_average_ratings(
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_DOCTOR_AVERAGE_RATINGS_HPP_
