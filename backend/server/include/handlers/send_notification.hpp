#pragma once

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../include/database.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

// Обработчик запросов на отправку SMS уведомлений
// Принимает JSON с полями:
// - phone_number: номер телефона получателя
// - message: текст сообщения
void send_notification_handler(
    const json &body,
    http::response<http::string_body> &res,
    database_handler &db_handler
);
