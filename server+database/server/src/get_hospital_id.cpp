#include "../include/get_hospital_id.hpp"
#include "../include/database.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <sstream>

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler* global_db;

int get_hospital_id_admin(int admin_id) {
    std::string admin_id_str = std::to_string(admin_id);
    const char *params[1] = {admin_id_str.c_str()};
    PGresult *res = PQexecParams(
        global_db->get_connection(),
        "SELECT hospital_id FROM hospitals WHERE administrator_id = $1 LIMIT 1",
        1, nullptr, params, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return -1; 
    }
    int hospital_id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return hospital_id;
}
