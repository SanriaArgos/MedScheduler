#include "../../include/handlers/delete_doctor_feedback.hpp"
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include "../../include/check_doctor_hospital.hpp"
#include "../../include/get_hospital_id.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;
extern database_handler *global_db;

void delete_doctor_feedback(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;
    if (!data.contains("rating_id") || !data.contains("admin_id")) {
        response["success"] = false;
        response["error"] = "Missing rating_id or admin_id";
        res.result(http::status::bad_request);
    } else {
        int rid = data["rating_id"];
        int admin = data["admin_id"];
        // 1) Находим автора и врача для этого отзыва
        const char *chk1_p[1] = {std::to_string(rid).c_str()};
        PGresult *chk1 = PQexecParams(
            db_handler.get_connection(),
            "SELECT user_id, doctor_ref_id FROM rating WHERE id=$1::int", 1,
            nullptr, chk1_p, nullptr, nullptr, 0
        );
        if (!chk1 || PQntuples(chk1) == 0) {
            if (chk1) {
                PQclear(chk1);
            }
            response["success"] = false;
            response["error"] = "Feedback not found";
            res.result(http::status::not_found);
        } else {
            int author = std::stoi(PQgetvalue(chk1, 0, 0));
            int doc_id = std::stoi(PQgetvalue(chk1, 0, 1));
            PQclear(chk1);
            bool allowed = false;
            // 2) Автор может удалить
            if (data["admin_id"] == author) {
                allowed = true;
            } else {
                // 3) И junior admin может, если врач принадлежит его больнице
                int hosp_id = get_hospital_id_admin(admin);
                if (hosp_id > 0 && check_doctor_hospital(doc_id, hosp_id)) {
                    allowed = true;
                }
            }
            if (!allowed) {
                response["success"] = false;
                response["error"] = "Forbidden";
                res.result(http::status::forbidden);
            } else {
                // 4) Удаляем
                PGresult *del = PQexecParams(
                    db_handler.get_connection(),
                    "DELETE FROM rating WHERE id=$1::int", 1, nullptr, chk1_p,
                    nullptr, nullptr, 0
                );
                if (del && PQresultStatus(del) == PGRES_COMMAND_OK) {
                    response["success"] = true;
                    res.result(http::status::ok);
                } else {
                    response["success"] = false;
                    response["error"] = "Delete failed";
                    res.result(http::status::internal_server_error);
                }
                if (del) {
                    PQclear(del);
                }
            }
        }
    }
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
