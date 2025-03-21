#include "../../include/handlers/display_hospitals.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

json display_hospitals_table(database_handler &db) {
    PGconn *conn = db.get_connection();
    json hospitals_json = json::array(); 

    std::string query = "SELECT hospital_id, region, settlement_type, settlement_name, street, house, full_name, administrator_id FROM hospitals ORDER BY hospital_id";
    PGresult *res = PQexec(conn, query.c_str());

    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Ошибка вывода таблицы больниц: " << PQerrorMessage(conn) << "\n";
        if (res) PQclear(res);
        return json{{"error", "Ошибка при выполнении запроса"}}; 
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        json hospital = {
            {"hospital_id", PQgetvalue(res, i, 0)},
            {"region", PQgetvalue(res, i, 1)},
            {"settlement_type", PQgetvalue(res, i, 2)},
            {"settlement_name", PQgetvalue(res, i, 3)},
            {"street", PQgetvalue(res, i, 4)},
            {"house", PQgetvalue(res, i, 5)},
            {"full_name", PQgetvalue(res, i, 6)},
            {"administrator_id", PQgetvalue(res, i, 7)}
        };
        hospitals_json.push_back(hospital); 
    }

    PQclear(res);
    return hospitals_json; 
}