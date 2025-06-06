#include "../../include/handlers/search_doctors.hpp"
#include <iostream>
#include <libpq-fe.h>
#include <sstream>
#include <vector>

void search_doctors(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    json response;

    try {
        std::cerr << "Получен запрос search_doctors" << std::endl;
        
        // 1) Считываем фильтры и флаг сортировки
        std::string region = data.value("region", "-");
        std::string settlement_type = data.value("settlement_type", "-");
        std::string settlement_name = data.value("settlement_name", "-");
        std::string full_name = data.value("full_name", "-");
        std::string specialty = data.value("specialty", "-");
        bool sort_by_rating = data.value("sort_by_rating", false);

        std::cerr << "Параметры запроса: " 
                  << "region=" << region << ", "
                  << "settlement_type=" << settlement_type << ", "
                  << "settlement_name=" << settlement_name << ", "
                  << "full_name=" << full_name << ", "
                  << "specialty=" << specialty << ", "
                  << "sort_by_rating=" << (sort_by_rating ? "true" : "false") 
                  << std::endl;

        // 2) Строим WHERE-условия и параметры
        std::vector<std::string> clauses;
        // Сохраняем копии строк параметров, чтобы избежать проблем с указателями
        std::vector<std::string> paramStrings;
        std::vector<const char *> paramValues;
        int idx = 1;

        // Изменим условия для hospital_ids, чтобы обрабатывать NULL значения и пустые массивы
        if (region != "-") {
            clauses.push_back(
                "EXISTS (SELECT 1 FROM hospitals h "
                " WHERE h.hospital_id = ANY(COALESCE(d.hospital_ids, ARRAY[]::integer[])) "
                "   AND h.region = $" +
                std::to_string(idx) + "::text)"
            );
            paramStrings.push_back(region);
            paramValues.push_back(paramStrings.back().c_str());
            idx++;
        }
        if (settlement_type != "-") {
            clauses.push_back(
                "EXISTS (SELECT 1 FROM hospitals h "
                " WHERE h.hospital_id = ANY(COALESCE(d.hospital_ids, ARRAY[]::integer[])) "
                "   AND h.settlement_type = $" +
                std::to_string(idx) + "::text)"
            );
            paramStrings.push_back(settlement_type);
            paramValues.push_back(paramStrings.back().c_str());
            idx++;
        }
        if (settlement_name != "-") {
            clauses.push_back(
                "EXISTS (SELECT 1 FROM hospitals h "
                " WHERE h.hospital_id = ANY(COALESCE(d.hospital_ids, ARRAY[]::integer[])) "
                "   AND h.settlement_name = $" +
                std::to_string(idx) + "::text)"
            );
            paramStrings.push_back(settlement_name);
            paramValues.push_back(paramStrings.back().c_str());
            idx++;
        }
        if (full_name != "-") {
            clauses.push_back(
                "EXISTS (SELECT 1 FROM hospitals h "
                " WHERE h.hospital_id = ANY(COALESCE(d.hospital_ids, ARRAY[]::integer[])) "
                "   AND h.full_name = $" +
                std::to_string(idx) + "::text)"
            );
            paramStrings.push_back(full_name);
            paramValues.push_back(paramStrings.back().c_str());
            idx++;
        }
        if (specialty != "-") {
            clauses.push_back("d.specialty = $" + std::to_string(idx) + "::text");
            paramStrings.push_back(specialty);
            paramValues.push_back(paramStrings.back().c_str());
            idx++;
        }

        // 3) Собираем SQL
        std::ostringstream sql;
        sql << "SELECT "
               "d.doctor_id, "
               "u.last_name || ' ' || u.first_name || "
               "COALESCE(' ' || u.patronymic, '') AS fio, "
               "d.specialty, COALESCE(d.experience, 0) AS experience, COALESCE(d.price, 0) AS price, "
               "COALESCE(avg_r.avg_rate, 0) AS average_rate "
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

        std::string sqlQuery = sql.str();
        std::cerr << "SQL запрос: " << sqlQuery << std::endl;
        std::cerr << "Количество параметров: " << paramValues.size() << std::endl;

        // 5) Выполняем запрос
        PGresult *pgres = PQexecParams(
            db_handler.get_connection(), sqlQuery.c_str(), static_cast<int>(paramValues.size()),
            nullptr, paramValues.empty() ? nullptr : paramValues.data(), nullptr,
            nullptr, 0
        );
        
        if (!pgres) {
            std::cerr << "Ошибка подключения к базе данных" << std::endl;
            throw std::runtime_error("Failed to execute query: connection error");
        }
        
        auto status = PQresultStatus(pgres);
        if (status != PGRES_TUPLES_OK) {
            std::string errorMsg = PQresultErrorMessage(pgres);
            std::cerr << "Ошибка SQL: " << errorMsg << " (статус: " << status << ")" << std::endl;
            PQclear(pgres);
            throw std::runtime_error("Database error: " + errorMsg);
        }

        // 6) Формируем JSON
        int rows = PQntuples(pgres);
        std::cerr << "Найдено врачей: " << rows << std::endl;
        
        json doctors = json::array();
        for (int i = 0; i < rows; ++i) {
            json doc;
            
            // Добавляем проверки на NULL и безопасное преобразование
            try {
                doc["doctor_id"] = PQgetisnull(pgres, i, 0) ? 0 : std::stoi(PQgetvalue(pgres, i, 0));
                doc["fio"] = PQgetisnull(pgres, i, 1) ? "" : PQgetvalue(pgres, i, 1);
                doc["specialty"] = PQgetisnull(pgres, i, 2) ? "" : PQgetvalue(pgres, i, 2);
                doc["experience"] = PQgetisnull(pgres, i, 3) ? 0 : std::stoi(PQgetvalue(pgres, i, 3));
                doc["price"] = PQgetisnull(pgres, i, 4) ? 0 : std::stoi(PQgetvalue(pgres, i, 4));
                doc["average_rate"] = PQgetisnull(pgres, i, 5) ? 0.0 : std::stod(PQgetvalue(pgres, i, 5));
                
                doctors.push_back(doc);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при обработке строки " << i << ": " << e.what() << std::endl;
                // Пропускаем некорректную запись и продолжаем
            }
        }
        PQclear(pgres);

        // 7) Отдаём ответ
        response["success"] = true;
        response["doctors"] = std::move(doctors);
        res.result(http::status::ok);
        std::cerr << "Ответ успешно сформирован" << std::endl;
        
    } catch (const std::exception& e) {
        // Логируем ошибку и формируем ответ с ошибкой
        std::cerr << "Критическая ошибка в search_doctors: " << e.what() << std::endl;
        response["success"] = false;
        response["error"] = std::string("Error processing request: ") + e.what();
        res.result(http::status::internal_server_error);
    }
    
    res.set(http::field::content_type, "application/json");
    res.body() = response.dump();
    std::cerr << "Отправка ответа клиенту" << std::endl;
}
