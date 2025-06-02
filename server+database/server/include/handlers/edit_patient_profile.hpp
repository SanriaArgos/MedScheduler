#ifndef EDIT_PATIENT_PROFILE_HPP_
#define EDIT_PATIENT_PROFILE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /edit_patient_profile
// В body должен прийти JSON вида:
// {
//   "user_id": <int>,
//   "current_password": "<string>",
//
//   // Любые из этих полей (необязательные):
//   "last_name": "<string>",
//   "first_name": "<string>",
//   "patronymic": "<string>",
//   "phone": "<string>",
//
//   // Если меняем пароль, то эти три поля обязательно:
//   "new_password": "<string>",
//   "new_password_repeat": "<string>"
// }
//
// ✎ Пациент может менять только: last_name, first_name, patronymic, phone, пароль.
// ✎ При смене пароля в таблице users будут обновлены поля hashed_password и salt.
void edit_patient_profile(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // EDIT_PATIENT_PROFILE_HPP_
