#ifndef JUNIOR_ADMIN_SCHEDULE_HPP_
#define JUNIOR_ADMIN_SCHEDULE_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция возвращает JSON с расписанием врача для младшего администратора.
// Ожидается, что входной JSON содержит поля "junior_admin_id" и "doctor_id".
void junior_admin_schedule(const json &data, http::response<http::string_body> &res, database_handler &db_handler);

static bool get_junior_admin_hospital_id(int junior_admin_id, int &out_hospital_id);


#endif  // JUNIOR_ADMIN_SCHEDULE_HPP_