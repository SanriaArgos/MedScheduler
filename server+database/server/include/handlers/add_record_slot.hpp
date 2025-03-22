#ifndef ADD_RECORD_SLOT_HPP_
#define ADD_RECORD_SLOT_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Функция добавления слота записи принимает JSON с полями:
// doctor_id, date, time, hospital_id, cabinet, junior_admin_id.
void add_record_slot(const json &data, http::response<http::string_body> &res, database_handler &db_handler);
static int get_junior_admin_hospital_id(int junior_admin_id);

#endif  // ADD_RECORD_SLOT_HPP_