#include "../../include/handlers/get_waitlist.hpp"
#include <libpq-fe.h>
#include <iostream>
#include "../../include/check_doctor_hospital.hpp"
#include "../../include/get_hospital_id.hpp"

void get_waitlist(
    int doctor_id,
    int junior_admin_id,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1. Проверка подключения к БД
    if (PQstatus(db_handler.get_connection()) != CONNECTION_OK) {
        response["success"] = false;
        response["error"] = "Database connection failed";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 2. Получаем hospital_id администратора
    int hospital_id = get_hospital_id_admin(junior_admin_id);
    if (hospital_id < 0) {
        response["success"] = false;
        response["error"] = "Your hospital not found";
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 3. Проверяем привязку врача к больнице
    if (!check_doctor_hospital(doctor_id, hospital_id)) {
        response["success"] = false;
        response["error"] = "Doctor is not associated with your hospital";
        res.result(http::status::forbidden);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 4. Формируем и выполняем SQL-запрос
    std::string doc_str = std::to_string(doctor_id);
    const char *params[1] = {doc_str.c_str()};

    PGresult *pgres = PQexecParams(
        db_handler.get_connection(),
        "SELECT id, patient_id, last_name, first_name, patronymic, phone, "
        "requested_at "  // Исправленное имя поля
        "FROM waitlist "
        "WHERE doctor_id = $1::int "
        "ORDER BY requested_at",  // Исправленное имя поля
        1, nullptr, params, nullptr, nullptr, 0
    );

    // 5. Обработка ошибок запроса
    if (!pgres) {
        response["success"] = false;
        response["error"] = "Database query failed (NULL result)";
        res.result(http::status::internal_server_error);
    } else if (PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        response["success"] = false;
        response["error"] =
            "Database error: " + std::string(PQresultErrorMessage(pgres));
        res.result(http::status::internal_server_error);
        PQclear(pgres);
    } else {
        // 6. Успешный случай: парсим результат
        int rows = PQntuples(pgres);
        json waitlist = json::array();

        for (int i = 0; i < rows; ++i) {
            json item;
            item["id"] = std::stoi(PQgetvalue(pgres, i, 0));
            item["patient_id"] = std::stoi(PQgetvalue(pgres, i, 1));
            item["last_name"] = PQgetvalue(pgres, i, 2);
            item["first_name"] = PQgetvalue(pgres, i, 3);
            item["patronymic"] = PQgetvalue(pgres, i, 4);
            item["phone"] = PQgetvalue(pgres, i, 5);
            item["request_date"] =
                PQgetvalue(pgres, i, 6);  // Можно оставить старое имя в JSON
            waitlist.push_back(std::move(item));
        }
        PQclear(pgres);

        response["success"] = true;
        response["waitlist"] = std::move(waitlist);
        res.result(http::status::ok);
    }

    // 7. Отправка ответа
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}