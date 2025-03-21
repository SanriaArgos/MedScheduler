#include "user_menu.hpp"
#include "utils.hpp"
#include "patient_schedule.hpp"  // Функция для показа расписания врачей для пациента
#include <iostream>

bool user_menu() {
    std::cerr << "User menu is not available in JSON mode. Use the JSON interface instead.\n";
    return false;
}
