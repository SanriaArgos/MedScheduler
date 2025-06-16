#ifndef TAKE_WAITLIST_SLOT_HPP_
#define TAKE_WAITLIST_SLOT_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void take_waitlist_slot(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif
