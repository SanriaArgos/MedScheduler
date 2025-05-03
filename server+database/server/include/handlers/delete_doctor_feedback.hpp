#ifndef DELETE_DOCTOR_FEEDBACK_HPP_
#define DELETE_DOCTOR_FEEDBACK_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

void delete_doctor_rating(
    const nlohmann::json &data,
    boost::beast::http::response<boost::beast::http::string_body> &res,
    database_handler &db_handler
);

#endif  // DELETE_DOCTOR_RATING_HPP_