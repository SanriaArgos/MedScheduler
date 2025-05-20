#include "../../include/handlers/get_cancelled_slots.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

void get_cancelled_slots(
    int doctor_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    res.set(http::field::content_type, "application/json");

    std::string doctor_id_str = std::to_string(doctor_id);
    const char *param_values[1] = {doctor_id_str.c_str()};

    PGresult *result = PQexecParams(
        db_handler.get_connection(),
        "SELECT record_id, appointment_date, appointment_time "
        "FROM records WHERE doctor_id=$1::int AND cancelled=TRUE",
        1, nullptr, param_values, nullptr, nullptr, 0
    );

    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] = "Database error";

        res.result(http::status::internal_server_error);
        res.body() = response.dump();
        res.prepare_payload();  // <== ОБЯЗАТЕЛЬНО!

        if (result) {
            PQclear(result);
        }
        return;  // <== Не продолжаем выполнение!
    }

    json slots = json::array();
    for (int i = 0; i < PQntuples(result); ++i) {
        json slot;
        slot["record_id"] = std::stoi(PQgetvalue(result, i, 0));
        slot["date"] = PQgetvalue(result, i, 1);
        slot["time"] = PQgetvalue(result, i, 2);
        slots.push_back(std::move(slot));
    }

    response["success"] = true;
    response["slots"] = std::move(slots);

    res.result(http::status::ok);
    res.body() = response.dump();
    res.prepare_payload();  // <== ОБЯЗАТЕЛЬНО!

    PQclear(result);
}
