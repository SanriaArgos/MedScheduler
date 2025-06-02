#ifndef POST_DOCTOR_RATING_HPP_
#define POST_DOCTOR_RATING_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

void post_doctor_rating(
    const nlohmann::json &data,
    boost::beast::http::response<boost::beast::http::string_body> &res,
    database_handler &db_handler
);

#endif  // POST_DOCTOR_RATING_HPP_