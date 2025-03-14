#include "handlers.hpp"
#include <iostream>
#include <string>
#include "database.hpp"
#include "login.hpp"
#include "registration.hpp"

std::string
registration_handler(database_handler &db, const std::string &request_data) {
    registration(db);
    return "registration handler executed";
}

std::string
login_handler(database_handler &db, const std::string &request_data) {
    std::string result = login(db);
    return "login handler executed: " + result;
}

std::string
records_handler(database_handler &db, const std::string &request_data) {
    std::string records =
        db.get_patient_records(1);  // пример для patient_id = 1
    return "records handler executed: " + records;
}
