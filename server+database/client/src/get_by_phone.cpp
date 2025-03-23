// #include "get_by_phone.hpp"
// #include "common_for_all.hpp"
// #include <iostream>
// #include <string>
// #include <curl/curl.h>
// #include <nlohmann/json.hpp>

// // Объявление вашей existing функции
// std::string send_get_request(const std::string& url);

// int get_user_id_by_phone(const std::string& phone) {
//     // URL‑encode номера
//     char *escaped = curl_easy_escape(nullptr, phone.c_str(), 0);
//     std::string url = "http://localhost:8080/get_user_id?phone=" + std::string(escaped);
//     curl_free(escaped);

//     std::string resp = send_get_request(url);
//     try {
//         auto j = nlohmann::json::parse(resp);
//         if (j.contains("id")) {
//             return j["id"].get<int>();
//         } else {
//             std::cerr << "get_user_id error: " << j.value("error", "unknown") << "\n";
//         }
//     } catch (const std::exception &e) {
//         std::cerr << "JSON parse error in get_user_id: " << e.what() << "\n";
//     }
//     return -1;
// }

// std::string get_user_type_by_phone(const std::string& phone) {
//     char *escaped = curl_easy_escape(nullptr, phone.c_str(), 0);
//     std::string url = "http://localhost:8080/get_user_type?phone=" + std::string(escaped);
//     curl_free(escaped);

//     std::string resp = send_get_request(url);
//     try {
//         auto j = nlohmann::json::parse(resp);
//         if (j.contains("user_type")) {
//             return j["user_type"].get<std::string>();
//         } else {
//             std::cerr << "get_user_type error: " << j.value("error", "unknown") << "\n";
//         }
//     } catch (const std::exception &e) {
//         std::cerr << "JSON parse error in get_user_type: " << e.what() << "\n";
//     }
//     return "";
// }
