#ifndef DELETE_SELF_ACCOUNT_HPP_
#define DELETE_SELF_ACCOUNT_HPP_

#include "../database.hpp"
#include <nlohmann/json.hpp>
#include <boost/beast/http.hpp>

namespace http  = boost::beast::http;
using   json  = nlohmann::json;

// POST /delete_self_account
// { "user_id": <int> }
// Пользователь типа patient или doctor может удалить свой аккаунт и все связанные данные.
// Другие типы (junior/senior admin) здесь не обрабатываются.
void delete_self_account(
    const json &data,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif // DELETE_SELF_ACCOUNT_HPP_
