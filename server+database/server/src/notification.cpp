#include <iostream>
#include <string>
#include <stdexcept> // Для std::runtime_error
#include <curl/curl.h> // Заголовочный файл libcurl

// Объявляем константы для данных аутентификации SMS API
const std::string SMS_API_LOGIN = "89128870195";
const std::string SMS_API_PASSWORD = "medscheduler";

// Callback функция для обработки ответа от сервера
// libcurl будет вызывать эту функцию для записи полученных данных
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



// Функция для отправки SMS с указанными учетными данными
// Принимает:
// - login: Ваш логин для API
// - password: Ваш пароль для API
// - phoneNumber: Номер телефона получателя
// - textMessage: Текст сообщения
// Возвращает: Строку с ответом от сервера
std::string sendSmsWithAuth(const std::string& login, const std::string& password, const std::string& phoneNumber, const std::string& textMessage) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer; // Буфер для хранения ответа сервера

    // Формируем XML тело запроса
    std::string xmlPayload = "<?xml version='1.0' encoding='UTF-8'?>"
                             "<data>"
                             "<login>" + login + "</login>"
                             "<password>" + password + "</password>"
                             "<action>send</action>"
                             "<text>" + textMessage + "</text>"
                             "<to number='" + phoneNumber + "'></to>"
                             "</data>";

    std::string url = "http://lcab.sms-sending.ru/API/XML/send.php";

    curl_global_init(CURL_GLOBAL_ALL); // Инициализация libcurl (глобально)
    curl = curl_easy_init(); // Получение дескриптора сессии curl

    if(curl) {
        // Устанавливаем URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Указываем, что это POST запрос
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Передаем XML данные
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xmlPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)xmlPayload.length());

        // Устанавливаем заголовки (важно для XML)
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: text/xml; charset=UTF-8"); // или application/xml
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Устанавливаем callback функцию для записи ответа
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Таймаут для операции
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 секунд

        // Отключаем проверку SSL сертификата (как в PHP примере, но не рекомендуется для продакшена)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Выполняем запрос
        res = curl_easy_perform(curl);

        // Проверяем на ошибки
        if(res != CURLE_OK) {
            std::string errorMsg = "curl_easy_perform() failed: ";
            errorMsg += curl_easy_strerror(res);
            curl_easy_cleanup(curl); // Очистка сессии curl
            curl_slist_free_all(headers); // Очистка списка заголовков
            curl_global_cleanup(); // Глобальная очистка libcurl
            throw std::runtime_error(errorMsg);
        }

        // Очистка
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        curl_global_cleanup();
        throw std::runtime_error("Failed to initialize libcurl session.");
    }

    curl_global_cleanup(); // Глобальная очистка libcurl

    return readBuffer; // Возвращаем ответ сервера
}


// Функция для отправки SMS
// Принимает:
// - phoneNumber: Номер телефона получателя
// - textMessage: Текст сообщения
// Возвращает: Строку с ответом от сервера
std::string sendSms(const std::string& phoneNumber, const std::string& textMessage) {
    return sendSmsWithAuth(SMS_API_LOGIN, SMS_API_PASSWORD, phoneNumber, textMessage);
}