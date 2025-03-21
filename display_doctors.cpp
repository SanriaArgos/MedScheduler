#include "display_doctors.hpp"
#include <iostream>
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern database_handler* global_db;

json display_doctors_table(const json &data) {
    json result;
    PGconn *conn = global_db->get_connection();
    PGresult *res = PQexecParams(conn,
        "SELECT d.doctor_id, d.user_id, u.last_name, u.first_name, u.patronymic, u.phone, d.education, d.specialty, d.experience "
        "FROM doctors d JOIN users u ON d.user_id = u.id ORDER BY d.doctor_id",
        0, nullptr, nullptr, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        result["success"] = false;
        result["error"] = "Doctors table unavailable";
        if (res) PQclear(res);
        return result;
    }
    int rows = PQntuples(res);
    json doctors = json::array();
    for (int i = 0; i < rows; ++i) {
        json doc;
        doc["doctor_id"] = PQgetvalue(res, i, 0);
        doc["user_id"] = PQgetvalue(res, i, 1);
        doc["last_name"] = PQgetvalue(res, i, 2);
        doc["first_name"] = PQgetvalue(res, i, 3);
        doc["patronymic"] = PQgetvalue(res, i, 4);
        doc["phone"] = PQgetvalue(res, i, 5);
        doc["education"] = PQgetvalue(res, i, 6);
        doc["specialty"] = PQgetvalue(res, i, 7);
        doc["experience"] = PQgetvalue(res, i, 8);
        doctors.push_back(doc);
    }
    PQclear(res);
    result["success"] = true;
    result["doctors"] = doctors;
    return result;
}
