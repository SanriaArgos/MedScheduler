#ifndef REQUEST_WAITLIST_HPP_
#define REQUEST_WAITLIST_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /request_waitlist
// { doctor_id, patient_id, last_name, first_name, patronymic, phone }
void add_patient_to_waitlist(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // REQUEST_WAITLIST_HPP_
