#include "../../include/handlers/display_doctors.hpp"
#include <iostream>
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

void display_doctors_table(const json &data, http::response<http::string_body> &res, database_handler &db_handler) {
    json result;

    PGconn *conn = db_handler.get_connection();
    PGresult *res_query = PQexecParams(conn,
        "SELECT d.doctor_id, d.user_id, u.last_name, u.first_name, u.patronymic, u.phone, d.education, d.specialty, d.experience "
        "FROM doctors d JOIN users u ON d.user_id = u.id ORDER BY d.doctor_id",
        0, nullptr, nullptr, nullptr, nullptr, 0);

    if (!res_query || PQresultStatus(res_query) != PGRES_TUPLES_OK) {
        result["success"] = false;
        result["error"] = "Doctors table unavailable";

        res.result(http::status::internal_server_error);  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = result.dump();

        if (res_query) PQclear(res_query);
        return;
    }

    int rows = PQntuples(res_query);
    json doctors = json::array();
    for (int i = 0; i < rows; ++i) {
        json doc;
        doc["doctor_id"] = PQgetvalue(res_query, i, 0);
        doc["user_id"] = PQgetvalue(res_query, i, 1);
        doc["last_name"] = PQgetvalue(res_query, i, 2);
        doc["first_name"] = PQgetvalue(res_query, i, 3);
        doc["patronymic"] = PQgetvalue(res_query, i, 4);
        doc["phone"] = PQgetvalue(res_query, i, 5);
        doc["education"] = PQgetvalue(res_query, i, 6);
        doc["specialty"] = PQgetvalue(res_query, i, 7);
        doc["experience"] = PQgetvalue(res_query, i, 8);
        doctors.push_back(doc);
    }

    PQclear(res_query);

    result["success"] = true;
    result["doctors"] = doctors;

    res.result(http::status::ok);  // 200 OK
    res.set(http::field::content_type, "application/json");
    res.body() = result.dump();
}