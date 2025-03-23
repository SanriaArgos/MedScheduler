#include "common_for_all.hpp"

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t new_length = size * nmemb;
    try {
        s->append((char*)contents, new_length);
    } catch (std::bad_alloc& e) {
        return 0;
    }
    return new_length;
}

std::string send_get_request(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        // Устанавливаем URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Устанавливаем callback-функцию для записи ответа
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Выполняем запрос
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Ошибка cURL: " << curl_easy_strerror(res) << std::endl;
        }

        // Очищаем ресурсы
        curl_easy_cleanup(curl);
    }
    return response;
}

std::string send_post_request(const std::string& url, const nlohmann::json& json_data) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init(); // Инициализируем CURL
    if (curl) {
        // Устанавливаем URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Преобразуем JSON в строку
        std::string post_data = json_data.dump();

        // Устанавливаем данные для POST-запроса
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

        // Устанавливаем callback-функцию для записи ответа
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Устанавливаем заголовки
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Выполняем запрос
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Ошибка cURL: " << curl_easy_strerror(res) << std::endl;
        }

        // Очищаем ресурсы
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        //вернуть строку в которой айди и тип юзера из http response
    }
    return response;
}