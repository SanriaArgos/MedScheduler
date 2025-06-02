#ifndef EDIT_SENIOR_ADMIN_PROFILE_HPP_
#define EDIT_SENIOR_ADMIN_PROFILE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /edit_senior_admin_profile
// Body JSON:
// {
//   "user_id":<int>,
//   "current_password":"<string>",
//   // разрешено менять:
//   "last_name": "<string>",
//   "first_name": "<string>",
//   "patronymic": "<string>",
//   "phone": "<string>",
//   // если меняем пароль, обязательно:
//   "new_password":"<string>",
//   "new_password_repeat":"<string>"
// }
//
// ✎ Старший админ может менять: last_name, first_name, patronymic, phone, пароль.
void edit_senior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // EDIT_SENIOR_ADMIN_PROFILE_HPP_
