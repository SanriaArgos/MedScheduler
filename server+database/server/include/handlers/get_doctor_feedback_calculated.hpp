#ifndef GET_DOCTOR_FEEDBACK_CALCULATED_HPP_
#define GET_DOCTOR_FEEDBACK_CALCULATED_HPP_

#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include "../database.hpp"

void get_doctor_rating_calculated(
    const nlohmann::json &data,
    boost::beast::http::response<boost::beast::http::string_body> &res,
    database_handler &db_handler
);

#endif // GET_DOCTOR_FEEDBACK_CALCULATED_HPP_