#include "../../include/handlers/assign_junior_admin_to_hospital.hpp"
#include <libpq-fe.h>

void assign_junior_admin_to_hospital(
    const json& data,
    http::response<http::string_body>& res,
    database_handler& db_handler
) {
    json response;
    if (!data.contains("junior_admin_id") || !data.contains("hospital_id")) {
        response["success"] = false;
        response["error"]   = "Missing junior_admin_id or hospital_id";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    int junior_id = data["junior_admin_id"];
    int hosp_id   = data["hospital_id"];

    // Обновим hospital.administrator_id
    const char* params[2] = {
        std::to_string(junior_id).c_str(),
        std::to_string(hosp_id)  .c_str()
    };
    PGresult* pgres = PQexecParams(
        db_handler.get_connection(),
        "UPDATE hospitals SET administrator_id = $1 WHERE hospital_id = $2",
        2, nullptr, params, nullptr, nullptr, 0
    );
    if (!pgres || PQresultStatus(pgres) != PGRES_COMMAND_OK) {
        if (pgres) PQclear(pgres);
        response["success"] = false;
        response["error"]   = PQerrorMessage(db_handler.get_connection());
        res.result(http::status::internal_server_error);
    } else {
        PQclear(pgres);
        response["success"] = true;
        response["message"] = "Junior admin assigned to hospital";
        res.result(http::status::ok);
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
