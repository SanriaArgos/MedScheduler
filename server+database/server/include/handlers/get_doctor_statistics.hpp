#ifndef GET_DOCTOR_STATISTICS_HPP_
#define GET_DOCTOR_STATISTICS_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /get_doctor_statistics
// Body JSON:
// {
//   "junior_admin_id": <int>,
//   "doctor_phone": "<string>"
// }
//
// Возвращает для указанного врача (по телефону), прикрепленного к той же
// больнице, что и младший администратор, следующие данные за текущий месяц:
//   - patients_count: количество пациентов (записей) у этого врача в его
//   больнице
//   - average_rating: среднее значение рейтинга из таблицы rating (округлённое
//   до десятых)
//   - price: цена приёма в таблице doctors
//
// Если врач не прикреплен к больнице данного администратора или не найден —
// возвращает ошибку.
void get_doctor_statistics(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // GET_DOCTOR_STATISTICS_HPP_
