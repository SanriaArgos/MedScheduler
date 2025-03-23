#ifndef ADD_DOCTOR_HPP_
#define ADD_DOCTOR_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

using json = nlohmann::json;

// Функция добавления врача принимает JSON с полями:
// last_name, first_name, [patronymic], phone, education, specialty, experience.
void add_doctor(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // ADD_DOCTOR_HPP_