#ifndef TAKE_WAITLIST_SLOT_HPP_
#define TAKE_WAITLIST_SLOT_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void take_waitlist_slot(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif
