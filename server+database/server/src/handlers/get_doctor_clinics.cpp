#include "../../include/handlers/get_doctor_clinics.hpp"
#include <libpq-fe.h>

void get_doctor_clinics(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    std::string did = std::to_string(doctor_id);
    const char *params[1] = {did.c_str()};

    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        R"(
            SELECT h.hospital_id,
                   h.region,
                   h.settlement_type,
                   h.settlement_name,
                   h.street,
                   h.house,
                   h.full_name,
                   h.time_open,
                   h.junior_admin_phone
              FROM doctors d
              JOIN LATERAL unnest(d.hospital_ids) AS hid(id) ON true
              JOIN hospitals h ON h.hospital_id = hid.id
             WHERE d.doctor_id = $1
             ORDER BY h.full_name
        )",
        1, nullptr, params, nullptr, nullptr, 0
    );

    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) {
            PQclear(pgres);
        }
        response = {{"success", false}, {"error", "Cannot fetch clinics"}};
        res.result(http::status::internal_server_error);
    } else {
        json arr = json::array();
        for (int i = 0; i < PQntuples(pgres); ++i) {
            json item;
            item["hospital_id"] = std::stoi(PQgetvalue(pgres, i, 0));
            item["region"] = PQgetvalue(pgres, i, 1);
            item["settlement_type"] = PQgetvalue(pgres, i, 2);
            item["settlement_name"] = PQgetvalue(pgres, i, 3);
            item["street"] = PQgetvalue(pgres, i, 4);
            item["house"] = PQgetvalue(pgres, i, 5);
            item["full_name"] = PQgetvalue(pgres, i, 6);
            item["time_open"] = PQgetvalue(pgres, i, 7);
            item["junior_admin_phone"] = PQgetvalue(pgres, i, 8);
            arr.push_back(item);
        }
        PQclear(pgres);
        response = {{"success", true}, {"clinics", arr}};
        res.result(http::status::ok);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
