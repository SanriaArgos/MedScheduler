#ifndef DISPLAY_DOCTORS_HPP_
#define DISPLAY_DOCTORS_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

json display_doctors_table(const json &data);

#endif  // DISPLAY_DOCTORS_HPP_
