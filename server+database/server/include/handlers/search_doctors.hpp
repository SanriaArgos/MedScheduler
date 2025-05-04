#ifndef SEARCH_DOCTORS_HPP_
#define SEARCH_DOCTORS_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /search_doctors
// Тело JSON может содержать любые из полей:
//   region, settlement_type, settlement_name, full_name, specialty, rate
//
// В ответ отдаётся:
// {
//   "success": true,
//   "doctors": [
//     {
//       "fio": <string>,
//       "specialty": <string>,
//       "experience": <int>,
//       "price": <int>,
//       "average_rate": <double>
//     },
//     …
//   ]
// }
void search_doctors(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // SEARCH_DOCTORS_HPP_
