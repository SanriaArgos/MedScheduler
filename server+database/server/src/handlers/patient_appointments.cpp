#include "../../include/handlers/patient_appointments.hpp"
#include <libpq-fe.h>
#include <sstream>

void patient_appointments(
    const http::request<http::string_body>& req,
    http::response<http::string_body>& res,
    database_handler& db_handler
) {
    // 1) Вытаскиваем patient_id из URL
    auto target = req.target().to_string();
    auto pos = target.find("patient_id=");
    json response;
    if (pos == std::string::npos) {
        response["success"] = false;
        response["error"]   = "Missing patient_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int patient_id = std::stoi(target.substr(pos+11));

    // 2) SQL — JOIN records↔hospitals↔users(админ)↔doctors↔users(врач)
    std::ostringstream sql;
    sql << R"(
      SELECT
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
     WHERE r.patient_id = )" << patient_id << R"(
     ORDER BY r.appointment_date, r.appointment_time
    )";

    PGresult *pgres = PQexec(db_handler.get_connection(), sql.str().c_str());
    if (!pgres || PQresultStatus(pgres)!=PGRES_TUPLES_OK) {
        if (pgres) PQclear(pgres);
        response["success"] = false;
        response["error"]   = "DB error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    json arr = json::array();
    for (int i=0; i<PQntuples(pgres); ++i) {
        json r;
        r["appointment_date"]     = PQgetvalue(pgres,i,0);
        r["appointment_time"]     = PQgetvalue(pgres,i,1);
        r["region"]               = PQgetvalue(pgres,i,2);
        r["settlement_type"]      = PQgetvalue(pgres,i,3);
        r["settlement_name"]      = PQgetvalue(pgres,i,4);
        r["street"]               = PQgetvalue(pgres,i,5);
        r["house"]                = PQgetvalue(pgres,i,6);
        r["full_name"]            = PQgetvalue(pgres,i,7);
        r["junior_admin_phone"]   = PQgetvalue(pgres,i,8);
        r["specialty"]            = PQgetvalue(pgres,i,9);
        r["price"]                = PQgetvalue(pgres, i, 10);
        r["doctor_name"]          = PQgetvalue(pgres,i,10);
        arr.push_back(std::move(r));
    }
    PQclear(pgres);

    response["success"]      = true;
    response["appointments"] = std::move(arr);
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
