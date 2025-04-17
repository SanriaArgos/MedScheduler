#include "../include/check_doctor_hospital.hpp"
#include "../include/database.hpp"
#include <libpq-fe.h>
#include <string>

extern database_handler* global_db;

bool check_doctor_hospital(int doctor_id, int admin_hospital_id) {
    std::string doctor_id_str = std::to_string(doctor_id);
    std::string hospital_id_str = std::to_string(admin_hospital_id);
    const char* params[2] = {doctor_id_str.c_str(), hospital_id_str.c_str()};

    // возвращаем 1, если заданный hospital_id содержится в массиве hospital_ids для данного доктора
    PGresult *res = PQexecParams(
        global_db->get_connection(),
        "SELECT 1 FROM doctors WHERE doctor_id = $1 AND $2 = ANY(hospital_ids) LIMIT 1",
        2,            // число параметров
        nullptr,      // типы параметров 
        params,
        nullptr,      // длина параметров
        nullptr,      // форматы параметров
        0             // текстовый режим
    );

    // Если запрос выполнился некорректно или строк не найдено – возвращаем false.
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}
