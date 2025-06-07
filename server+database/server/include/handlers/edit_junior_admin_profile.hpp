#ifndef EDIT_JUNIOR_ADMIN_PROFILE_HPP_
#define EDIT_JUNIOR_ADMIN_PROFILE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /edit_junior_admin_profile
// Body JSON:
// {
//   "user_id":<int>,
//   "current_password":"<string>",
//   // разрешено менять только:
//   "last_name": "<string>",
//   "first_name": "<string>",
//   "patronymic": "<string>",
//   // если меняем пароль, обязательно:
//   "new_password":"<string>",
//   "new_password_repeat":"<string>"
// }
//
// ✎ Младший админ может менять: last_name, first_name, patronymic и пароль.
// ✎ Телефон ему менять нельзя.
void edit_junior_admin_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // EDIT_JUNIOR_ADMIN_PROFILE_HPP_
