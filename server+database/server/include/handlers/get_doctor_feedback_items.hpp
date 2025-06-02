#ifndef GET_DOCTOR_FEEDBACK_ITEMS_HPP_
#define GET_DOCTOR_FEEDBACK_ITEMS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

void get_doctor_rating_items(
    const nlohmann::json &data,
    boost::beast::http::response<boost::beast::http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_DOCTOR_FEEDBACK_ITEMS_HPP_