#ifndef DISPLAY_HOSPITALS_HPP
#define DISPLAY_HOSPITALS_HPP

#include <nlohmann/json.hpp>
#include "../database.hpp"

using json = nlohmann::json;

json display_hospitals_table(database_handler &db);

#endif // DISPLAY_HOSPITALS_HPP