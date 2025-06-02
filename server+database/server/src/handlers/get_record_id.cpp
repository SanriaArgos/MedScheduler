#include "../../include/handlers/get_record_id.hpp"
#include <libpq-fe.h>

int get_record_id_for_patient_and_doctor(
    int doctor_id,
    int patient_id,
    database_handler &db_handler
) {
    // Параметры для SQL-запроса
    const char *params[2] = {
        std::to_string(doctor_id).c_str(),
        std::to_string(patient_id).c_str()
    };
    // Ищем slot, в котором doctor_id и patient_id совпадают и отмена ещё не проставлена
    PGresult *res = PQexecParams(
        db_handler.get_connection(),
        "SELECT record_id "
        "FROM records "
        "WHERE doctor_id = $1::int "
        "  AND patient_id = $2::int "
        "  AND cancelled = FALSE "
        "LIMIT 1",
        2,       // число параметров
        nullptr, // типы (текстовый режим)
        params,
        nullptr,
        nullptr,
        0        // текстовый результат
    );
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (res) PQclear(res);
        return -1;
    }
    // Парсим найденный record_id
    int record_id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return record_id;
}
