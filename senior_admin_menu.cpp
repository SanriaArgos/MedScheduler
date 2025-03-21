#include "senior_admin_menu.hpp"
#include "system_init.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <openssl/sha.h>

bool senior_admin_menu() {
    std::cerr << "Senior administrator menu is not available in JSON mode. Use the JSON interface instead.\n";
    return false;
}
