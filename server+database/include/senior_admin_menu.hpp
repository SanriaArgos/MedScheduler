#ifndef SENIOR_ADMIN_MENU_HPP_
#define SENIOR_ADMIN_MENU_HPP_

#include "database.hpp"
#include <string>

void senior_admin_menu(database_handler &db, int admin_id);
void display_users_table(database_handler &db);
void display_hospitals_table(database_handler &db);
void add_junior_admin(database_handler &db);
void add_hospital(database_handler &db);

#endif  // SENIOR_ADMIN_MENU_HPP_