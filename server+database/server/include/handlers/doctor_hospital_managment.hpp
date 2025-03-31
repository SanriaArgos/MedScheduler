#ifndef DOCTOR_HOSPITAL_MANAGEMENT_HPP_
#define DOCTOR_HOSPITAL_MANAGEMENT_HPP_

#include <nlohmann/json.hpp>
#include "../database.hpp"
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

static bool get_junior_admin_hospital_id(int junior_admin_id, int &out_hospital_id);

// Функция добавления больницы в список врача принимает JSON с полями: doctor_id, hospital_id, junior_admin_id.
void add_hospital_to_doctor(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

// Функция удаления больницы из списка врача принимает JSON с полями: doctor_id, hospital_id, junior_admin_id.
void remove_hospital_from_doctor(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

#endif  // DOCTOR_HOSPITAL_MANAGEMENT_HPP_