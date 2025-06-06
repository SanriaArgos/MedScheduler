#include "../../include/handlers/get_patient_waitlist.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <sstream>

void get_patient_waitlist(int patient_id, http::response<http::string_body> &res, database_handler &db_handler) {
    PGconn* conn = db_handler.get_connection();
    if (PQstatus(conn) != CONNECTION_OK) {
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database connection failed"})";
        return;
    }

    std::string query = "SELECT w.id, w.doctor_id, w.requested_at, "
                        "d.specialty, "
                        "u.last_name, u.first_name, u.patronymic, "
                        "h.full_name AS hospital_name, h.region, h.settlement_type, h.settlement_name, h.street, h.house "
                        "FROM waitlist w "
                        "JOIN doctors d ON w.doctor_id = d.doctor_id "
                        "JOIN users u ON d.user_id = u.id "
                        "JOIN LATERAL (SELECT h.* FROM hospitals h "
                        "                WHERE h.hospital_id = ANY(d.hospital_ids) "
                        "                LIMIT 1) h ON TRUE "
                        "WHERE w.patient_id = $1 "
                        "ORDER BY w.requested_at DESC";

    const char* param_values[1];
    std::string patient_id_str = std::to_string(patient_id);
    param_values[0] = patient_id_str.c_str();

    PGresult* result = PQexecParams(conn, query.c_str(), 1, nullptr, param_values, nullptr, nullptr, 0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::string err_msg = PQerrorMessage(conn);
        PQclear(result);
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"success": false, "error": "Database query failed: )" + err_msg + R"("})";
        return;
    }

    json waitlist_items = json::array();
    int num_rows = PQntuples(result);

    for (int i = 0; i < num_rows; i++) {
        json item;
        item["waitlist_id"] = std::stoi(PQgetvalue(result, i, 0));
        item["doctor_id"] = std::stoi(PQgetvalue(result, i, 1));
        item["requested_at"] = PQgetvalue(result, i, 2);
        item["doctor_specialty"] = PQgetvalue(result, i, 3);

        std::string last_name = PQgetvalue(result, i, 4);
        std::string first_name = PQgetvalue(result, i, 5);
        std::string doctor_name = last_name + " " + first_name;

        if (!PQgetisnull(result, i, 6)) {
            std::string patronymic = PQgetvalue(result, i, 6);
            doctor_name += " " + patronymic;
        }
        item["doctor_name"] = doctor_name;

        item["hospital_name"] = PQgetvalue(result, i, 7);

        std::string region = PQgetvalue(result, i, 8);
        std::string settlement_type = PQgetvalue(result, i, 9);
        std::string settlement_name = PQgetvalue(result, i, 10);
        std::string street = PQgetvalue(result, i, 11);
        std::string house = PQgetvalue(result, i, 12);

        std::string address = region + ", " + settlement_type + " " +
                            settlement_name + ", " + street + " " + house;
        item["hospital_address"] = address;

        waitlist_items.push_back(item);
    }

    PQclear(result);

    json response;
    response["success"] = true;
    response["patient_id"] = patient_id;
    response["waitlist_items"] = waitlist_items;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
