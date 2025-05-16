#ifndef CANCEL_WAITLIST_HPP_
#define CANCEL_WAITLIST_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

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
