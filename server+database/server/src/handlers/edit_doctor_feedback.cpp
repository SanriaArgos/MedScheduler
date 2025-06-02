#include "../../include/handlers/edit_doctor_feedback.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler *global_db;

void edit_doctor_feedback(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    // 1) Проверка полей
    if (!data.contains("rating_id") || !data.contains("user_id") ||
        !data.contains("rate") || !data.contains("name") ||
        !data.contains("date") || !data.contains("address")) {
        response["success"] = false;
        response["error"] = "Missing required fields";
        res.result(http::status::bad_request);
    } else {
        int rid = data["rating_id"];
        int uid = data["user_id"];
        int rate = data["rate"];
        std::string text = data.value("text", "");
        std::string name = data["name"];
        std::string date = data["date"];
        std::string address = data["address"];
        // 2) Проверяем авторство
        const char *chk_p[1] = {std::to_string(rid).c_str()};
        PGresult *chk = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_id FROM rating WHERE id=$1::int", 1, nullptr, chk_p,
            nullptr, nullptr, 0
        );
        if (!chk || PQntuples(chk) == 0) {
            if (chk) {
                PQclear(chk);
            }
            response["success"] = false;
            response["error"] = "Feedback not found";
            res.result(http::status::not_found);
        } else {
            int author = std::stoi(PQgetvalue(chk, 0, 0));
            PQclear(chk);
            if (author != uid) {
                response["success"] = false;
                response["error"] = "Forbidden";
                res.result(http::status::forbidden);
            } else {
                // 3) Выполняем UPDATE
                const char *upd_p[6] = {
                    text.c_str(),    name.c_str(),
                    date.c_str(),    std::to_string(rate).c_str(),
                    address.c_str(), std::to_string(rid).c_str()};
                PGresult *upd = PQexecParams(
                    db_handler.get_connection(),
                    "UPDATE rating SET "
                    "text=$1,name=$2,date=$3,rate=$4,address=$5 "
                    "WHERE id=$6::int",
                    6, nullptr, upd_p, nullptr, nullptr, 0
                );
                if (upd && PQresultStatus(upd) == PGRES_COMMAND_OK) {
                    response["success"] = true;
                    res.result(http::status::ok);
                } else {
                    response["success"] = false;
                    response["error"] = "Update failed";
                    res.result(http::status::internal_server_error);
                }
                if (upd) {
                    PQclear(upd);
                }
            }
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
