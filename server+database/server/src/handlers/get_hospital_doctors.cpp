#include "../../include/handlers/get_hospital_doctors.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_hospital_doctors(int hospital_id, http::response<http::string_body> &res, database_handler &db_handler) {
    PGconn* conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database connection failed"})";
        return;
    }

    std::string query = "SELECT d.doctor_id, d.user_id, u.last_name, u.first_name, "
                        "u.patronymic, u.phone, d.education, d.specialty, d.experience, "
                        "d.price "
                        "FROM doctors d "
                        "JOIN users u ON d.user_id = u.id "
                        "JOIN doctor_hospitals dh ON d.doctor_id = dh.doctor_id "
                        "WHERE dh.hospital_id = $1 "
                        "ORDER BY d.doctor_id";

    const char* param_values[1];
    std::string hospital_id_str = std::to_string(hospital_id);
    param_values[0] = hospital_id_str.c_str();

    PGresult* result = PQexecParams(conn, query.c_str(), 1, nullptr, param_values, nullptr, nullptr, 0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::string err_msg = PQerrorMessage(conn);
        PQclear(result);
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database query failed: )" + err_msg + R"("})";
        return;
    }

    json response;
    json doctors = json::array();
    int rows = PQntuples(result);
    
    for (int i = 0; i < rows; ++i) {
        json doctor;
        doctor["doctor_id"] = PQgetvalue(result, i, 0);
        doctor["user_id"] = PQgetvalue(result, i, 1);
        doctor["last_name"] = PQgetvalue(result, i, 2);
        doctor["first_name"] = PQgetvalue(result, i, 3);
        doctor["patronymic"] = PQgetvalue(result, i, 4);
        doctor["phone"] = PQgetvalue(result, i, 5);
        doctor["education"] = PQgetvalue(result, i, 6);
        doctor["specialty"] = PQgetvalue(result, i, 7);
        doctor["experience"] = PQgetvalue(result, i, 8);
        doctor["price"] = PQgetvalue(result, i, 9);
        doctors.push_back(doctor);
    }

    PQclear(result);

    response["success"] = true;
    response["doctors"] = doctors;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
