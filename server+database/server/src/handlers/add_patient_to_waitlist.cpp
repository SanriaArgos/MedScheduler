#include "../../include/handlers/add_patient_to_waitlist.hpp"
#include <libpq-fe.h>
#include <iostream>

void add_patient_to_waitlist(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    for (auto &f : {"doctor_id","patient_id","last_name","first_name","patronymic","phone"}) {
        if (!data.contains(f)) {
            response["success"] = false;
            response["error"] = std::string("Missing field '") + f + "'";
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
    }

    std::string doctor_id_str = std::to_string(data["doctor_id"].get<int>());
    std::string patient_id_str = std::to_string(data["patient_id"].get<int>());
    std::string last_name_user_str  = data["last_name"].get<std::string>();
    std::string first_name_user_str  = data["first_name"].get<std::string>();
    std::string middle_name_user_str  = data["patronymic"].get<std::string>();
    std::string phone_user_str  = data["phone"].get<std::string>();

    const char *params[6] = {
        doctor_id_str.c_str(), patient_id_str.c_str(),
        last_name_user_str.c_str(), first_name_user_str.c_str(), middle_name_user_str.c_str(), phone_user_str.c_str()
    };
    PGresult *r = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO waitlist "
        "(doctor_id, patient_id, last_name, first_name, patronymic, phone) "
        "VALUES ($1,$2,$3,$4,$5,$6)",
        6,nullptr,params,nullptr,nullptr,0
    );
    if (!r || PQresultStatus(r)!=PGRES_COMMAND_OK) {
        if(r)PQclear(r);
        response["success"] = false;
        response["error"]   = "Cannot request waitlist";
        res.result(http::status::internal_server_error);
    } else {
        PQclear(r);
        response["success"] = true;
        response["message"] = "Waitlist entry created";
        res.result(http::status::ok);
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
