#ifndef HANDLERS_HPP_
#define HANDLERS_HPP_

#include <string>
#include "database.hpp"

std::string registration_handler(
    const database_handler &db,
    const std::string &request_data
);
std::string
login_handler(const database_handler &db, const std::string &request_data);
std::string
records_handler(const database_handler &db, const std::string &request_data);

#endif  // HANDLERS_HPP_
