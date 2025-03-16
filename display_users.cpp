#include "display_users.hpp"
#include <iostream>
#include <libpq-fe.h>

void display_users_table(database_handler &db) {
    PGconn *conn = db.get_connection();
    std::cout << "\n=== Users Table ===\n";
    std::string query = "SELECT id, last_name, first_name, patronymic, phone, user_type FROM users ORDER BY id";
    PGresult *res = PQexec(conn, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Users table unavailable\n";
        if (res) PQclear(res);
        return;
    }
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        std::cout << "(" << PQgetvalue(res, i, 0) << ") " 
                  << PQgetvalue(res, i, 1) << "_" 
                  << PQgetvalue(res, i, 2) << "_" 
                  << PQgetvalue(res, i, 3) << ", " 
                  << PQgetvalue(res, i, 4) << ", " 
                  << PQgetvalue(res, i, 5) << "\n";
    }
    PQclear(res);
}