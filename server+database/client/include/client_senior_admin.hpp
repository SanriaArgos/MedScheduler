#ifndef CLIENT_SENIOR_ADMIN_HPP
#define CLIENT_SENIOR_ADMIN_HPP

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

void add_junior_admin(const json& data);
void add_hospital(const json& data);

void display_hospitals_table();
void display_users_table();

#endif // CLIENT_SENIOR_ADMIN_HPP