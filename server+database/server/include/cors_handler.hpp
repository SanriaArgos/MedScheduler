#pragma once

#include <boost/beast/http.hpp>
#include <string>

namespace http = boost::beast::http;

/**
 * Добавляет необходимые CORS-заголовки к HTTP-ответу
 * 
 * @param res Ссылка на HTTP-ответ
 */
template <class Body>
void add_cors_headers(http::response<Body>& res) {
    // Разрешаем запросы со всех источников (можно ограничить конкретным доменом)
    res.set(http::field::access_control_allow_origin, "*");
    
    // Разрешаем различные методы HTTP
    res.set(http::field::access_control_allow_methods, "GET, POST, PUT, DELETE, PATCH, OPTIONS");
    
    // Разрешаем заголовки, обычно используемые в запросах
    res.set(http::field::access_control_allow_headers, 
            "Content-Type, Authorization, X-Requested-With, Accept");
    
    // Время кеширования результатов preflight-запроса (в секундах)
    res.set(http::field::access_control_max_age, "60"); // 60 секунд
}

/**
 * Создает ответ на preflight OPTIONS запрос с CORS-заголовками
 * 
 * @return HTTP-ответ на preflight запрос
 */
inline http::response<http::empty_body> 
create_options_response(const http::request<http::string_body>& req) {
    http::response<http::empty_body> res{http::status::no_content, req.version()};
    add_cors_headers(res);
    res.prepare_payload();
    return res;
}
