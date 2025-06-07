#ifndef ASSIGN_JUNIOR_ADMIN_TO_HOSPITAL_HPP
#define ASSIGN_JUNIOR_ADMIN_TO_HOSPITAL_HPP

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// POST /assign_junior_admin_to_hospital
// Body JSON: { "junior_admin_id": int, "hospital_id": int }
// Привязывает младшего админа к указанной больнице (обновляет
// hospitals.administrator_id)
void assign_junior_admin_to_hospital(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // ASSIGN_JUNIOR_ADMIN_TO_HOSPITAL_HPP
