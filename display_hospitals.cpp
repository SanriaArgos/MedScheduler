#include "display_hospitals.hpp"
#include <iostream>
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern database_handler* global_db;

json display_hospitals_table(const json &data) {
    json result;
    PGconn *conn = global_db->get_connection();
    PGresult *res = PQexecParams(conn,
        "SELECT hospital_id, region, settlement_type, settlement_name, street, house, full_name, administrator_id FROM hospitals ORDER BY hospital_id",
        0, nullptr, nullptr, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        result["success"] = false;
        result["error"] = "Hospitals table unavailable";
        if (res) PQclear(res);
        return result;
    }
    int rows = PQntuples(res);
    json hospitals = json::array();
    for (int i = 0; i < rows; ++i) {
        json hosp;
        hosp["hospital_id"] = PQgetvalue(res, i, 0);
        hosp["region"] = PQgetvalue(res, i, 1);
        hosp["settlement_type"] = PQgetvalue(res, i, 2);
        hosp["settlement_name"] = PQgetvalue(res, i, 3);
        hosp["street"] = PQgetvalue(res, i, 4);
        hosp["house"] = PQgetvalue(res, i, 5);
        hosp["full_name"] = PQgetvalue(res, i, 6);
        hosp["administrator_id"] = PQgetvalue(res, i, 7);
        hospitals.push_back(hosp);
    }
    PQclear(res);
    result["success"] = true;
    result["hospitals"] = hospitals;
    return result;
}
