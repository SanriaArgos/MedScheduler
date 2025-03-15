#include "handlers.hpp"
#include "registration.hpp"
#include "login.hpp"
#include "database.hpp"
#include <iostream>
#include <string>

std::string registration_handler(database_handler &db) {
    registration(db);
    return "registration handler executed";
}

std::string login_handler(database_handler &db) {
    std::string result = login(db);
    return "login handler executed: " + result;
}

std::string records_handler(database_handler &db) {
    std::string records = db.get_patient_records(1); // пример для patient_id = 1
    return "records handler executed: " + records;
}