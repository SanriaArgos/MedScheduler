#include "../include/handlers/take_waitlist_slot.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

// Глобальный указатель на соединение с БД
extern database_handler *global_db;

// Обработчик «взятия» слота из очереди ожидания:
// Перемещает первого ожидающего пациента в только что освободившийся слот.
void take_waitlist_slot(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1) Проверяем, что все необходимые поля присутствуют
    if (!data.contains("doctor_id") || !data.contains("old_patient_id") ||
        !data.contains("new_patient_id") ||
        !data.contains("appointment_date") ||
        !data.contains("appointment_time") || !data.contains("hospital_id")) {
        response["success"] = false;
        response["error"] = "Missing required fields";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 2) Извлекаем параметры из JSON
    int doctor_id = data["doctor_id"].get<int>();
    int old_patient_id = data["old_patient_id"].get<int>();
    int new_patient_id = data["new_patient_id"].get<int>();
    std::string date = data["appointment_date"].get<std::string>();
    std::string time_val = data["appointment_time"].get<std::string>();
    int hospital_id = data["hospital_id"].get<int>();

    // 3) Обновляем запись: заменяем старого пациента на нового и сбрасываем
    // флаг cancelled
    {
        const char *params_update[6] = {
            std::to_string(new_patient_id).c_str(),
            std::to_string(old_patient_id).c_str(),
            std::to_string(doctor_id).c_str(),
            date.c_str(),
            time_val.c_str(),
            std::to_string(hospital_id).c_str()};
        PGresult *res_update = PQexecParams(
            db_handler.get_connection(),
            "UPDATE records "
            "SET patient_id = $1, cancelled = FALSE "
            "WHERE doctor_id = $3 AND appointment_date = $4 AND "
            "appointment_time = $5 "
            "  AND hospital_id = $6 AND patient_id = $2",
            6, nullptr, params_update, nullptr, nullptr, 0
        );
        if (PQresultStatus(res_update) != PGRES_COMMAND_OK) {
            std::cerr << "Error updating record: "
                      << PQerrorMessage(db_handler.get_connection()) << "\n";
            PQclear(res_update);
            response["success"] = false;
            response["error"] = "Error updating record";
            res.result(http::status::internal_server_error);
            res.set(http::field::content_type, "application/json");
            res.body() = response.dump();
            return;
        }
        PQclear(res_update);
    }

    // 4) Удаляем из таблицы waitlist запись только что обслуженного пациента
    {
        const char *params_del[2] = {
            std::to_string(doctor_id).c_str(),
            std::to_string(new_patient_id).c_str()};
        PGresult *res_del = PQexecParams(
            db_handler.get_connection(),
            "DELETE FROM waitlist WHERE doctor_id = $1 AND patient_id = $2", 2,
            nullptr, params_del, nullptr, nullptr, 0
        );
        if (res_del) {
            PQclear(res_del);
        }
    }

    // 5) Вызываем SQL-функцию notify_next_waitlister, чтобы спустить
    // уведомление следующему в очереди
    {
        const char *param_doc = std::to_string(doctor_id).c_str();
        PGresult *res_notify = PQexecParams(
            db_handler.get_connection(), "SELECT notify_next_waitlister($1)", 1,
            nullptr, &param_doc, nullptr, nullptr, 0
        );
        if (PQresultStatus(res_notify) != PGRES_TUPLES_OK) {
            std::cerr << "Error notifying next waitlister: "
                      << PQerrorMessage(db_handler.get_connection()) << "\n";
        }
        PQclear(res_notify);
    }

    // 6) Возвращаем успешный ответ
    response["success"] = true;
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
