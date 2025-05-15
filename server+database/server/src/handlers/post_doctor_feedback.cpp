#include "../../include/handlers/post_doctor_feedback.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler *global_db;

void post_doctor_rating(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    // 1) Проверяем обязательные поля
    if (!data.contains("doctor_ref_id") || !data.contains("user_id") ||
        !data.contains("rate") || !data.contains("name") ||
        !data.contains("date") || !data.contains("address")) {
        response["success"] = false;
        response["error"] = "Missing required fields";
        res.result(http::status::bad_request);
    } else {
        int doc_id = data["doctor_ref_id"];
        int usr_id = data["user_id"];
        int rate = data["rate"];
        std::string text = data.value("text", "");
        std::string name = data["name"];
        std::string date = data["date"];
        std::string address = data["address"];

        // 2) Проверяем, что у этого пользователя ещё нет отзыва к этому врачу
        const char *chk_params[2] = {
            std::to_string(doc_id).c_str(), std::to_string(usr_id).c_str()};
        PGresult *chk = PQexecParams(
            db_handler.get_connection(),
            "SELECT 1 FROM rating WHERE doctor_ref_id=$1::int AND "
            "user_id=$2::int",
            2, nullptr, chk_params, nullptr, nullptr, 0
        );
        bool exists = (chk && PQntuples(chk) > 0);
        PQclear(chk);
        if (exists) {
            response["success"] = false;
            response["error"] = "Feedback already exists";
            res.result(http::status::conflict);
        } else {
            // 3) Вставляем отзыв
            const char *ins_params[7] = {
                std::to_string(doc_id).c_str(),
                std::to_string(usr_id).c_str(),
                text.c_str(),
                name.c_str(),
                date.c_str(),
                std::to_string(rate).c_str(),
                address.c_str()};
            PGresult *ins = PQexecParams(
                db_handler.get_connection(),
                "INSERT INTO rating "
                "(doctor_ref_id,user_id,text,name,date,rate,address) "
                "VALUES($1,$2,$3,$4,$5,$6,$7) RETURNING id",
                7, nullptr, ins_params, nullptr, nullptr, 0
            );
            if (ins && PQresultStatus(ins) == PGRES_TUPLES_OK &&
                PQntuples(ins) == 1) {
                int new_id = std::stoi(PQgetvalue(ins, 0, 0));
                response["success"] = true;
                response["id"] = new_id;
                res.result(http::status::created);
            } else {
                response["success"] = false;
                response["error"] = "Insert failed";
                res.result(http::status::internal_server_error);
            }
            if (ins) {
                PQclear(ins);
            }
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
