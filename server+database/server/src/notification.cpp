#include <iostream>
#include <string>
#include <stdexcept>
#include <curl/curl.h>

// Используем API-токен вместо логина/пароля
const std::string SMS_API_TOKEN = "s45c3vodot5wjutvk91lbv8w7z2v58zt878zmsrmgeav8vealk81hgv8nj7z7zwv"; // Замените на реальный токен!

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string sendSmsWithToken(const std::string& phoneNumber, const std::string& textMessage) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    // Формируем XML с API-токеном (вариант 1)
    std::string xmlPayload = "<?xml version='1.0' encoding='UTF-8'?>"
                            "<data>"
                            "<api_key>" + SMS_API_TOKEN + "</api_key>"
                            "<action>send</action>"
                            "<text>" + textMessage + "</text>"
                            "<to number='" + phoneNumber + "'></to>"
                            "</data>";

    // Или используем Bearer Token в заголовке (вариант 2)
    // std::string xmlPayload = "<?xml version='1.0'?><data><action>send</action><text>...</text><to number='...'/></data>";

    std::string url = "http://lcab.sms-sending.ru/API/XML/send.php";

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xmlPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)xmlPayload.length());

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: text/xml; charset=UTF-8");
        
        // Если используете Bearer Token (раскомментируйте):
        // headers = curl_slist_append(headers, ("Authorization: Bearer " + SMS_API_TOKEN).c_str());
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::string errorMsg = "curl_easy_perform() failed: ";
            errorMsg += curl_easy_strerror(res);
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            throw std::runtime_error(errorMsg);
        }

        // Проверяем код ошибки в XML-ответе
        if (readBuffer.find("<code>501</code>") != std::string::npos || 
            readBuffer.find("<code>5") != std::string::npos) {
            throw std::runtime_error("SMS API Error: " + readBuffer);
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        curl_global_cleanup();
        throw std::runtime_error("Failed to initialize libcurl session.");
    }

    curl_global_cleanup();
    return readBuffer;
}

// Обновленная функция sendSms
std::string sendSms(const std::string& phoneNumber, const std::string& textMessage) {
    return sendSmsWithToken(phoneNumber, textMessage);
}