#ifndef DELETE_USER_BY_PHONE_HPP_
#define DELETE_USER_BY_PHONE_HPP_

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

/*
 POST /delete_user_by_phone
 {
   "phone": "<string>",    // телефон пользователя, которого удаляем
   "admin_id": <int>       // id того, кто делает удаление
 }

 Удаление по типу пользователя:
 1) patient       – удаляем его и все связанные записи
 2) junior admin  – удаляем его, его больницу, чистим hospital_ids у врачей
 3) doctor        – удаляем его и все связанные записи
 Преференция: только senior administrator может вызывать этот метод.
*/
void delete_user_by_phone(
    const std::string& phone,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // DELETE_USER_BY_PHONE_HPP_
