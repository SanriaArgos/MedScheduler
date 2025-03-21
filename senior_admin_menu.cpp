#include "senior_admin_menu.hpp"
#include "system_init.hpp"
#include <libpq-fe.h>
#include <iostream>

bool senior_admin_menu(const nlohmann::json &data) {
    std::cerr << "senior_admin_menu: client-side menu removed. Use JSON interface instead.\n";
    return false;
}
