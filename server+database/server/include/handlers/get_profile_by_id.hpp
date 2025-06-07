#ifndef GET_PROFILE_BY_ID_HPP
#define GET_PROFILE_BY_ID_HPP

#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /get_profile_by_id?user_id=...
// Возвращает {"success":true,"user":{...}} или {"success":false,"error":...}
void get_profile_by_id(
    int user_id,
    http::response<http::string_body>& res,
    database_handler& db_handler
);

#endif // GET_PROFILE_BY_ID_HPP
