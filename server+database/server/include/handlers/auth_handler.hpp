#ifndef AUTH_HANDLER_HPP_
#define AUTH_HANDLER_HPP_

#include "database.hpp"
#include <string>

std::string login_user(database_handler &db, const std::string &phone, const std::string &password);
bool register_user(database_handler &db,
                   const std::string &last_name,
                   const std::string &first_name,
                   const std::string &patronymic,
                   const std::string &phone,
                   const std::string &password);

int get_user_id_by_phone(database_handler &db, const std::string &phone);
std::string get_user_type_by_phone(database_handler &db,const std::string &phone);
bool user_exists(database_handler &db, const std::string &phone);

#endif  // AUTH_HANDLER_HPP_
