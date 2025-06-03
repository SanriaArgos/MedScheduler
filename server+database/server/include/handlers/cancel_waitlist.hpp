#ifndef CANCEL_WAITLIST_HPP_
#define CANCEL_WAITLIST_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /cancel_waitlist
// body: { waitlist_id: int, patient_id: int }
void cancel_waitlist(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // CANCEL_WAITLIST_HPP_
