#include "../../include/handlers/add_doctor.hpp"
#include <libpq-fe.h>
#include <boost/beast/http.hpp>
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>
#include "../../include/handlers/auth_handler.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler *global_db;

void add_doctor(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    nlohmann::json response;

    // Проверка на наличие обязательных полей
    if (!data.contains("last_name") || !data.contains("first_name") ||
        !data.contains("phone") || !data.contains("education") ||
        !data.contains("specialty") || !data.contains("experience") ||
        !data.contains("price")) {
        std::cerr << "Error: Missing required fields for add_doctor\n";
        response["success"] = false;
        response["error"] = "Missing required fields";
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    std::string last_name = data["last_name"];
    std::string first_name = data["first_name"];
    std::string patronymic = data.value("patronymic", "");
    std::string phone = data["phone"];
    std::string education = data["education"];
    std::string specialty = data["specialty"];
    int experience = data["experience"];
    int price = data["price"].get<int>();

    // Проверка, если номер телефона уже зарегистрирован
    if (user_exists(db_handler, phone)) {
        std::cerr << "Error: Phone already registered\n";
        response["success"] = false;
        response["error"] = "Phone already registered";
        res.result(http::status::conflict);  // 409 Conflict
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    const std::string default_password = "987654321";

    // Регистрация пользователя
    if (!register_user(
            *global_db, last_name, first_name, patronymic, phone,
            default_password
        )) {
        std::cerr << "Error: Could not add doctor to Users table\n";
        response["success"] = false;
        response["error"] = "Could not add doctor to Users table";

        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // Обновление типа пользователя
    const char *params_update[1] = {phone.c_str()};
    PGresult *res_db = PQexecParams(
        db_handler.get_connection(),
        "UPDATE users SET user_type = 'doctor' WHERE phone = $1", 1, NULL,
        params_update, NULL, NULL, 0
    );
    if (PQresultStatus(res_db) != PGRES_COMMAND_OK) {
        std::cerr << "Error: Could not update user type: "
                  << PQerrorMessage(db_handler.get_connection()) << "\n";
        PQclear(res_db);
        response["success"] = false;
        response["error"] = "Could not update user type";

        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    PQclear(res_db);

    // Получение ID пользователя
    res_db = PQexecParams(
        db_handler.get_connection(), "SELECT id FROM users WHERE phone = $1", 1,
        NULL, params_update, NULL, NULL, 0
    );
    if (!res_db || PQresultStatus(res_db) != PGRES_TUPLES_OK ||
        PQntuples(res_db) == 0) {
        std::cerr << "Error: Could not retrieve doctor ID\n";
        if (res_db) {
            PQclear(res_db);
        }
        response["success"] = false;
        response["error"] = "Could not retrieve doctor ID";

        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }
    int user_id = static_cast<int>(std::stoi(PQgetvalue(res_db, 0, 0)));
    PQclear(res_db);

    // Добавление информации о докторе в таблицу Doctors
    std::string user_id_str = std::to_string(user_id);
    std::string experience_str = std::to_string(experience);
    std::string price_str = std::to_string(price);
    const char *params_ins[6] = {user_id_str.c_str(), education.c_str(),
                                 specialty.c_str(),   experience_str.c_str(),
                                 price_str.c_str(),   "{}"};
    res_db = PQexecParams(
        db_handler.get_connection(),
        "INSERT INTO doctors (user_id, education, specialty, experience, "
        "price, "
        "hospital_ids) VALUES ($1, $2, $3, $4, $5, $6)",
        6, NULL, params_ins, NULL, NULL, 0
    );
    if (PQresultStatus(res_db) == PGRES_COMMAND_OK) {
        std::cerr << "Doctor data successfully added to Doctors table\n";
        PQclear(res_db);
        response["success"] = true;
        response["message"] = "Doctor successfully added";

        res.result(http::status::ok);  // 200 OK
    } else {
        std::cerr << "Error: Could not add data to Doctors table\n";
        PQclear(res_db);
        response["success"] = false;
        response["error"] = "Could not add data to Doctors table";

        res.result(http::status::internal_server_error
        );  // 500 Internal Server Error
    }

    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
