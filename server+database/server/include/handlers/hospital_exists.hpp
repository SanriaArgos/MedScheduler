#ifndef HOSPITAL_EXISTS_HPP
#define HOSPITAL_EXISTS_HPP

#include <boost/beast/http.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;

void hospital_exists(int hospital_id, http::response<http::string_body>& res, database_handler& db_handler);

#endif // HOSPITAL_EXISTS_HPP
