#include "../../include/handlers/get_waitlist.hpp"
#include "../../include/get_hospital_id.hpp"
#include "../../include/check_doctor_hospital.hpp"
#include <libpq-fe.h>
#include <iostream>

void get_waitlist(
    int doctor_id,
    int junior_admin_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Получаем hospital_id данного младшего администратора
    int hospital_id = get_hospital_id_admin(junior_admin_id);
    if (hospital_id < 0) {
        response["success"] = false;
        response["error"]   = "Your hospital not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 2. Проверяем, что врач прикреплён к этой больнице
    if (!check_doctor_hospital(doctor_id, hospital_id)) {
        response["success"] = false;
        response["error"]   = "Doctor is not associated with your hospital";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 3. Запрашиваем все заявки для данного врача
    std::string doc_str = std::to_string(doctor_id);
    const char *params[1] = { doc_str.c_str() };
    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "SELECT id, patient_id, last_name, first_name, patronymic, phone, request_date "
        "FROM waitlist "
        "WHERE doctor_id = $1::int "
        "ORDER BY request_date",
        1, nullptr, params, nullptr, nullptr, 0
    );
    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) PQclear(pgres);
        response["success"] = false;
        response["error"]   = "Cannot fetch waitlist";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4. Собираем массив JSON-объектов по каждой заявке
    int rows = PQntuples(pgres);
    json waitlist = json::array();
    for (int i = 0; i < rows; ++i) {
        json item;
        item["id"]          = std::stoi(PQgetvalue(pgres, i, 0));
        item["patient_id"]  = std::stoi(PQgetvalue(pgres, i, 1));
        item["last_name"]   = PQgetvalue(pgres, i, 2);
        item["first_name"]  = PQgetvalue(pgres, i, 3);
        item["patronymic"]  = PQgetvalue(pgres, i, 4);
        item["phone"]       = PQgetvalue(pgres, i, 5);
        item["request_date"]= PQgetvalue(pgres, i, 6);
        waitlist.push_back(std::move(item));
    }
    PQclear(pgres);

    // 5. Возвращаем ответ
    response["success"]  = true;
    response["waitlist"] = std::move(waitlist);
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
