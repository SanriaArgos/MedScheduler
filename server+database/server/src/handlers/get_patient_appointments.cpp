#include "../../include/handlers/get_patient_appointments.hpp"
#include <libpq-fe.h>
#include <sstream>

void get_patient_appointments(
    int patient_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    std::ostringstream sql;
    sql << R"(
      SELECT
        r.record_id,
        r.doctor_id,
        r.appointment_date,
        r.appointment_time,
        h.region,
        h.settlement_type,
        h.settlement_name,
        h.street,
        h.house,
        h.full_name,
        u1.phone        AS junior_admin_phone,
        d.specialty,
        d.price,
        u2.last_name || ' ' || u2.first_name || ' ' || COALESCE(u2.patronymic,'') AS doctor_name
      FROM records r
      JOIN hospitals h ON r.hospital_id = h.hospital_id
      JOIN users     u1 ON h.administrator_id = u1.id
      JOIN doctors   d  ON r.doctor_id = d.doctor_id
      JOIN users     u2 ON d.user_id = u2.id
     WHERE r.patient_id = $1
     ORDER BY r.appointment_date, r.appointment_time
    )";

    const char *paramValues[1] = {std::to_string(patient_id).c_str()};

    PGresult *pgres = PQexecParams(
        db_handler.get_connection(), sql.str().c_str(), 1, nullptr, paramValues,
        nullptr, nullptr, 0
    );

    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) {
            PQclear(pgres);
        }
        response["success"] = false;
        response["error"] = "Database error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    json appointments = json::array();
    for (int i = 0; i < PQntuples(pgres); ++i) {
        json record;
        record["record_id"] = std::stoi(PQgetvalue(pgres, i, 0));
        record["doctor_id"] = std::stoi(PQgetvalue(pgres, i, 1));
        record["appointment_date"] = PQgetvalue(pgres, i, 2);
        record["appointment_time"] = PQgetvalue(pgres, i, 3);
        record["region"] = PQgetvalue(pgres, i, 4);
        record["settlement_type"] = PQgetvalue(pgres, i, 5);
        record["settlement_name"] = PQgetvalue(pgres, i, 6);
        record["street"] = PQgetvalue(pgres, i, 7);
        record["house"] = PQgetvalue(pgres, i, 8);
        record["hospital_name"] = PQgetvalue(pgres, i, 9);
        record["admin_phone"] = PQgetvalue(pgres, i, 10);
        record["specialty"] = PQgetvalue(pgres, i, 11);
        record["price"] = PQgetvalue(pgres, i, 12);
        record["doctor_name"] = PQgetvalue(pgres, i, 13);

        appointments.push_back(std::move(record));
    }
    PQclear(pgres);

    response["success"] = true;
    response["appointments"] = appointments;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}