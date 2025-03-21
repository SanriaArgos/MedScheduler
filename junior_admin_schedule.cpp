#include "junior_admin_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <algorithm>
#include <string>

// Заглушка: интерактивное меню расписания удалено.
bool junior_admin_schedule(database_handler &db, int junior_admin_id) {
    std::cerr << "junior_admin_schedule: client-side schedule display removed. Use JSON interface instead.\n";
    return false;
}
