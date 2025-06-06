#include "../../include/handlers/search_doctors.hpp"
#include <libpq-fe.h>
#include <sstream>

void search_doctors(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    // 1) Считываем фильтры и флаг сортировки
    std::string region = data.value("region", "-");
    std::string settlement_type = data.value("settlement_type", "-");
    std::string settlement_name = data.value("settlement_name", "-");
    std::string full_name = data.value("full_name", "-");
    std::string specialty = data.value("specialty", "-");
    bool sort_by_rating = data.value("sort_by_rating", false);

    // 2) Строим WHERE-условия и параметры
    std::vector<std::string> clauses;
    std::vector<const char *> paramValues;
    int idx = 1;

    if (region != "-") {
        clauses.push_back(
            "EXISTS (SELECT 1 FROM hospitals h "
            " WHERE h.hospital_id = ANY(d.hospital_ids) "
            "   AND h.region = $" +
            std::to_string(idx) + "::text)"
        );
        paramValues.push_back(region.c_str());
        idx++;
    }
    if (settlement_type != "-") {
        clauses.push_back(
            "EXISTS (SELECT 1 FROM hospitals h "
            " WHERE h.hospital_id = ANY(d.hospital_ids) "
            "   AND h.settlement_type = $" +
            std::to_string(idx) + "::text)"
        );
        paramValues.push_back(settlement_type.c_str());
        idx++;
    }
    if (settlement_name != "-") {
        clauses.push_back(
            "EXISTS (SELECT 1 FROM hospitals h "
            " WHERE h.hospital_id = ANY(d.hospital_ids) "
            "   AND h.settlement_name = $" +
            std::to_string(idx) + "::text)"
        );
        paramValues.push_back(settlement_name.c_str());
        idx++;
    }
    if (full_name != "-") {
        clauses.push_back(
            "EXISTS (SELECT 1 FROM hospitals h "
            " WHERE h.hospital_id = ANY(d.hospital_ids) "
            "   AND h.full_name = $" +
            std::to_string(idx) + "::text)"
        );
        paramValues.push_back(full_name.c_str());
        idx++;
    }
    if (specialty != "-") {
        clauses.push_back("d.specialty = $" + std::to_string(idx) + "::text");
        paramValues.push_back(specialty.c_str());
        idx++;
    }

    // 3) Собираем SQL
    std::ostringstream sql;
    sql << "SELECT "
           "d.doctor_id, "
           "u.last_name || ' ' || u.first_name || "
           "COALESCE(' ' || u.patronymic, '') AS fio, "
           "d.specialty, d.experience, d.price, "
           "COALESCE(avg_r.avg_rate,0) AS average_rate "
        << "FROM doctors d "
           "JOIN users u ON u.id = d.user_id "
           "LEFT JOIN ("
           "SELECT doctor_ref_id, ROUND(AVG(rate)::numeric,1) AS avg_rate "
           "FROM rating GROUP BY doctor_ref_id"
           ") avg_r ON avg_r.doctor_ref_id = d.doctor_id";

    if (!clauses.empty()) {
        sql << " WHERE " << clauses[0];
        for (size_t i = 1; i < clauses.size(); ++i) {
            sql << " AND " << clauses[i];
        }
    }

    // 4) Добавляем ORDER BY
    if (sort_by_rating) {
        sql << " ORDER BY average_rate DESC NULLS LAST, fio";
    } else {
        sql << " ORDER BY fio";
    }

    // 5) Выполняем запрос
    PGresult *pgres = PQexecParams(
        db_handler.get_connection(), sql.str().c_str(), (int)paramValues.size(),
        nullptr, paramValues.empty() ? nullptr : paramValues.data(), nullptr,
        nullptr, 0
    );
    if (!pgres || PQresultStatus(pgres) != PGRES_TUPLES_OK) {
        if (pgres) {
            PQclear(pgres);
        }
        response["success"] = false;
        response["error"] = "Database error";
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();
        return;
    }

    // 6) Формируем JSON
    int rows = PQntuples(pgres);
    json doctors = json::array();
    for (int i = 0; i < rows; ++i) {
        json doc;
        doc["doctor_id"] = std::stoi(PQgetvalue(pgres, i, 0));  // doctor_id теперь первый столбецAdd commentMore actions
        doc["fio"] = PQgetvalue(pgres, i, 1);           // смещение остальных полей +1
        doc["specialty"] = PQgetvalue(pgres, i, 2);
        doc["experience"] = std::stoi(PQgetvalue(pgres, i, 3));
        doc["price"] = std::stoi(PQgetvalue(pgres, i, 4));
        doc["average_rate"] = std::stod(PQgetvalue(pgres, i, 5));
        doctors.push_back(std::move(doc));
    }
    PQclear(pgres);

    // 7) Отдаём ответ
    response["success"] = true;
    response["doctors"] = std::move(doctors);
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
}
