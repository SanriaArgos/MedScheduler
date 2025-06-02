#include "../../include/handlers/send_notification.hpp"
#include "../../include/notification.hpp"
#include <iostream>

void send_notification_handler(const json& body,
                             http::response<http::string_body>& res,
                             database_handler& db_handler) {
    try {
        // Проверяем наличие необходимых полей в запросе
        if (!body.contains("phone_number") || !body.contains("message")) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"success": false, "error": "Отсутствуют обязательные поля: phone_number и message"})";
            return;
        }

        // Получаем данные из запроса
        std::string phoneNumber = body["phone_number"];
        std::string message = body["message"];

        // Проверяем, что номер телефона не пустой
        if (phoneNumber.empty()) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"success": false, "error": "Номер телефона не может быть пустым"})";
            return;
        }

        // Проверяем, что сообщение не пустое
        if (message.empty()) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"success": false, "error": "Текст сообщения не может быть пустым"})";
            return;
        }

        // Отправляем SMS-уведомление, используя функцию из notification.cpp
        std::string smsResponse = sendSms(phoneNumber, message);

        // Создаем успешный ответ
        json response;
        response["success"] = true;
        response["message"] = "Уведомление успешно отправлено";
        response["service_response"] = smsResponse;

        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = response.dump();

    } catch (const std::exception& e) {
        // Обрабатываем любые ошибки, которые могут возникнуть
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        json error;
        error["success"] = false;
        error["error"] = "Ошибка при отправке уведомления: ";
        error["details"] = e.what();
        res.body() = error.dump();
    }
}
