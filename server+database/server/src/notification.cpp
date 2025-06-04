#include <iostream>
#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string SMS_API_TOKEN = "s45c3vodot5wjutvk91lbv8w7z2v58zt878zmsrmgeav8vealk81hgv8nj7z7zwv";
const std::string API_URL = "https://lcab.sms-sending.ru/json/v1.0/sms/send"; // Уточните точный endpoint

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string sendSms(const std::string& phoneNumber, const std::string& textMessage) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    // Формируем JSON запрос
    json requestBody = {
        {"to", phoneNumber},
        {"text", textMessage},
        {"sender", "MEDSCHEDULER"} // Укажите ваше имя отправителя
    };

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("X-Token: " + SMS_API_TOKEN).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, API_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::string errorMsg = "curl_easy_perform() failed: ";
            errorMsg += curl_easy_strerror(res);
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            throw std::runtime_error(errorMsg);
        }

        // Парсим JSON ответ
        try {
            json response = json::parse(readBuffer);
            if (!response["success"].get<bool>()) {
                throw std::runtime_error("API error: " + response["error"]["descr"].get<std::string>());
            }
        } catch (const json::exception& e) {
            throw std::runtime_error("Failed to parse API response: " + std::string(e.what()));
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