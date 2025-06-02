#ifndef GET_WAITLIST_COUNT_HPP_
#define GET_WAITLIST_COUNT_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /waitlist_count?doctor_id=<int>
// Возвращает {"success": true, "count": <число>} — сколько заявок перед новым
void get_waitlist_count(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_WAITLIST_COUNT_HPP_
