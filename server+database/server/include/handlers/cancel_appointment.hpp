// server/include/handlers/cancel_appointment.hpp
#ifndef CANCEL_APPOINTMENT_HPP_
#define CANCEL_APPOINTMENT_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void cancel_appointment(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif
