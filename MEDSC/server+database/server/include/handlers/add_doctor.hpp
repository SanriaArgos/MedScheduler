#ifndef ADD_DOCTOR_HPP_
#define ADD_DOCTOR_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;

using json = nlohmann::json;

// Функция добавления врача принимает JSON с полями:
// last_name, first_name, [patronymic], phone, education, specialty, experience.
void add_doctor(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // ADD_DOCTOR_HPP_