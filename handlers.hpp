#ifndef HANDLERS_HPP_
#define HANDLERS_HPP_

#include "database.hpp"
#include <string>

std::string registration_handler(database_handler &db);
std::string login_handler(database_handler &db);
std::string records_handler(database_handler &db);

#endif  // HANDLERS_HPP_
