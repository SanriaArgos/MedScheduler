#ifndef COMMON_FOR_ALL_HPP_
#define COMMON_FOR_ALL_HPP_

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* s);
std::string send_post_request(const std::string& url, const nlohmann::json& json_data);
std::string send_get_request(const std::string& url);


#endif //COMMON_FOR_ALL_HPP_