#ifndef DOCTOR_HANDLERS_HPP
#define DOCTOR_HANDLERS_HPP

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

void doctor_exists(int doctor_id, 
                      http::response<http::string_body>& res, 
                      database_handler& db_handler);

#endif // DOCTOR_HANDLERS_HPP