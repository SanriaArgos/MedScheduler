#ifndef EDIT_DOCTOR_RATING_HPP_
#define EDIT_DOCTOR_RATING_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

void edit_doctor_rating(
    const nlohmann::json &data,
    boost::beast::http::response<boost::beast::http::string_body> &res,
    database_handler &db_handler
);

#endif  // EDIT_DOCTOR_RATING_HPP_