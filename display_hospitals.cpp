#include "display_hospitals.hpp"
#include <iostream>
#include <libpq-fe.h>

void display_hospitals_table(database_handler &db) {
    PGconn *conn = db.get_connection();
    std::cout << "\n=== Hospitals Table ===\n";
    std::string query = "SELECT hospital_id, region, settlement_type, settlement_name, street, house, full_name, administrator_id FROM hospitals ORDER BY hospital_id";
    PGresult *res = PQexec(conn, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Hospitals table unavailable\n";
        if (res) PQclear(res);
        return;
    }
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        std::cout << "(" << PQgetvalue(res, i, 0) << ") " 
                  << PQgetvalue(res, i, 1) << "_" 
                  << PQgetvalue(res, i, 2) << "_" 
                  << PQgetvalue(res, i, 3) << "_" 
                  << PQgetvalue(res, i, 4) << "_" 
                  << PQgetvalue(res, i, 5) << "_" 
                  << PQgetvalue(res, i, 6) << "_" 
                  << PQgetvalue(res, i, 7) << "\n";
    }
    PQclear(res);
}
