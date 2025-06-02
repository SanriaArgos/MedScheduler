#include "../../include/handlers/book_appointment.hpp"
#include <libpq-fe.h>
#include <iostream>

void book_appointment(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    // 1. Проверка входных полей
    for (auto &f :
         {"doctor_id", "hospital_id", "appointment_date", "appointment_time",
          "patient_id"}) {
        if (!data.contains(f)) {
            response["success"] = false;
            response["error"] = std::string("Missing field '") + f + "'";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    // 2. Разбор
    std::string doctor_id_str = std::to_string(data["doctor_id"].get<int>());
    std::string hospital_id_str =
        std::to_string(data["hospital_id"].get<int>());
    std::string date_str = data["appointment_date"].get<std::string>();
    std::string time_str = data["appointment_time"].get<std::string>();
    std::string patient_id_str = std::to_string(data["patient_id"].get<int>());

    // 3. Убедимся, что слот существует
    const char *p1[4] = {
        doctor_id_str.c_str(), hospital_id_str.c_str(), date_str.c_str(),
        time_str.c_str()};
    PGresult *r1 = PQexecParams(
        db_handler.get_connection(),
        "SELECT patient_id "
        "FROM records "
        "WHERE doctor_id=$1::int "
        "  AND hospital_id=$2::int "
        "  AND appointment_date=$3::date "
        "  AND appointment_time=$4::time",
        4, nullptr, p1, nullptr, nullptr, 0
    );
    if (!r1 || PQresultStatus(r1) != PGRES_TUPLES_OK) {
        if (r1) {
            PQclear(r1);
        }
        response["success"] = false;
        response["error"] = "Cannot lookup slot";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    if (PQntuples(r1) == 0) {
        PQclear(r1);
        response["success"] = false;
        response["error"] = "Slot not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    // 4. Проверим, что ещё не занят
    const char *cur_pid = PQgetvalue(r1, 0, 0);
    if (cur_pid[0] != '\0') {
        PQclear(r1);
        response["success"] = false;
        response["error"] = "Slot already taken";
        res.result(http::status::conflict);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(r1);

    // 5. Ок, цепляем пациента
    const char *p2[5] = {
        doctor_id_str.c_str(), hospital_id_str.c_str(), date_str.c_str(),
        time_str.c_str(), patient_id_str.c_str()};
    PGresult *r2 = PQexecParams(
        db_handler.get_connection(),
        "UPDATE records SET patient_id = $5::int "
        " WHERE doctor_id=$1::int "
        "   AND hospital_id=$2::int "
        "   AND appointment_date=$3::date "
        "   AND appointment_time=$4::time",
        5, nullptr, p2, nullptr, nullptr, 0
    );
    if (!r2 || PQresultStatus(r2) != PGRES_COMMAND_OK) {
        if (r2) {
            PQclear(r2);
        }
        response["success"] = false;
        response["error"] = "Failed to book slot";
        res.result(http::status::internal_server_error);
    } else {
        PQclear(r2);
        response["success"] = true;
        response["message"] = "Booked successfully";
        res.result(http::status::ok);
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
