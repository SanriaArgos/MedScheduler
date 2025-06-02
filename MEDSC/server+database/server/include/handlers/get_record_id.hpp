#ifndef GET_RECORD_ID_HPP_
#define GET_RECORD_ID_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

// Ищет в таблице records незаблокированный (cancelled = FALSE) приём
// по заданному врачу и пациенту.
// Если запись найдена — возвращает её record_id, иначе — -1.
int get_record_id_for_patient_and_doctor(
    int doctor_id,
    int patient_id,
    database_handler &db_handler
);

#endif  // GET_RECORD_ID_HPP_
