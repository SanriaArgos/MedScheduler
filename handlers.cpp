#include "handlers.hpp"
#include "registration.hpp"
#include "login.hpp"
#include <iostream>
#include <string>

std::string registration_handler(database_handler &db) {
    registration(db);
    return "Registration executed";
}

std::string login_handler(database_handler &db) {
    std::string result = login(db);
    return "Login executed: " + result;
}

std::string records_handler(database_handler &db) {
    std::string records = db.get_patient_records(1); // пример
    return "Records executed: " + records;
}
