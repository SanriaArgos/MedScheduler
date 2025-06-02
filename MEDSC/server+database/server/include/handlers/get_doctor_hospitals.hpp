#ifndef GET_DOCTOR_HOSPITALS_HPP_
#define GET_DOCTOR_HOSPITALS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /doctors/{doctor_id}/hospitals
// Возвращает список клиник, в которых принимает врач:
// { success: true, hospitals: [ { hospital_id, region, settlement_type,
//   settlement_name, street, house, full_name, time_open, junior_admin_phone },
//   … ] }
void get_doctor_hospitals(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_DOCTOR_HOSPITALS_HPP_
