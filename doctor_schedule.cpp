#include "doctor_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <string>

bool display_doctor_schedule(database_handler &db, int doctor_id) {
    std::cerr << "display_doctor_schedule: client-side schedule display removed. Use JSON interface instead.\n";
    return false;
}
