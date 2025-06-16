#ifndef GET_WAITLIST_HPP_
#define GET_WAITLIST_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// GET /get_waitlist?doctor_id=<int>&junior_admin_id=<int>
// Возвращает список ожидания для данного врача, если врач прикреплён
// к больнице данного младшего администратора.
// Ответ:
// {
//   "success": true,
//   "waitlist": [
//     {
//       "id": <int>,
//       "patient_id": <int>,
//       "last_name": <string>,
//       "first_name": <string>,
//       "patronymic": <string>,
//       "phone": <string>,
//       "request_date": <string>
//     },
//     …
//   ]
// }
void get_waitlist(
    int doctor_id,
    int junior_admin_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_WAITLIST_HPP_
