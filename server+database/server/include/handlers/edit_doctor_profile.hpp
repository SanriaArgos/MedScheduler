#ifndef EDIT_DOCTOR_PROFILE_HPP_
#define EDIT_DOCTOR_PROFILE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /edit_doctor_profile
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
// ✎ Логика та же, что и у пациента (edit_patient_profile), но дополнительно
//   проверяем, что user_type = "doctor".
void edit_doctor_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // EDIT_DOCTOR_PROFILE_HPP_