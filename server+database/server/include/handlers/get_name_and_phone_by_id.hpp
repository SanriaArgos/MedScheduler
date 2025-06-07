#ifndef GET_NAME_AND_PHONE_BY_ID_HPP
#define GET_NAME_AND_PHONE_BY_ID_HPP

#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// Хэндлер для запроса profile пользователя по его id.
// Request Body: {"user_id": int}
// Response Body: {"success": true, "user": { ... }} или {"success": false, "error": "..."}
void get_name_and_phone_by_id(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif // GET_NAME_AND_PHONE_BY_ID_HPP
