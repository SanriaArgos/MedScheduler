#ifndef DISPLAY_HOSPITALS_HPP_
#define DISPLAY_HOSPITALS_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

json display_hospitals_table(const json &data);

#endif  // DISPLAY_HOSPITALS_HPP_
