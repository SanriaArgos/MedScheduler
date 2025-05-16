// server/include/handlers/cancel_appointment.hpp
#ifndef CANCEL_APPOINTMENT_HPP_
#define CANCEL_APPOINTMENT_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void cancel_appointment(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif
