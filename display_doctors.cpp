#include "display_doctors.hpp"
#include <iostream>
#include <libpq-fe.h>

void display_doctors_table(database_handler &db) {
    PGconn *conn = db.get_connection();
    std::cout << "\n=== Doctors Table ===\n";
    std::string query = "SELECT d.doctor_id, d.user_id, u.last_name, u.first_name, u.patronymic, u.phone, d.education, d.specialty, d.experience "
                        "FROM doctors d JOIN users u ON d.user_id = u.id ORDER BY d.doctor_id";
    PGresult *res = PQexec(conn, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Doctors table unavailable\n";
        if (res) PQclear(res);
        return;
    }
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        std::cout << "(" << PQgetvalue(res, i, 0) << ") " 
                  << PQgetvalue(res, i, 1) << "_" 
                  << PQgetvalue(res, i, 2) << "_" 
                  << PQgetvalue(res, i, 3) << "_" 
                  << PQgetvalue(res, i, 4) << ", " 
                  << PQgetvalue(res, i, 5) << ", " 
                  << PQgetvalue(res, i, 6) << "_" 
                  << PQgetvalue(res, i, 7) << "_" 
                  << PQgetvalue(res, i, 8) << "\n";
    }
    PQclear(res);
}
