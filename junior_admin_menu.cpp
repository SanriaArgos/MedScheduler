#include "junior_admin_menu.hpp"
#include "utils.hpp"
#include "display_users.hpp"
#include "display_doctors.hpp"
#include "doctor_hospital_management.hpp"  
#include "add_record_slot.hpp"
#include "junior_admin_schedule.hpp"
#include "add_doctor.hpp"
#include <iostream>

bool junior_admin_menu() {
    std::cerr << "Junior administrator menu is not available in JSON mode. Use the JSON interface instead.\n";
    return false;
}
