#ifndef COMMON_FOR_ALL_HPP_
#define COMMON_FOR_ALL_HPP_

#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

extern const std::string base_url;

size_t
write_callback(void *contents, size_t size, size_t nmemb, std::string *s);
std::string
send_post_request(const std::string &url, const nlohmann::json &json_data);
std::string send_get_request(const std::string &url);

std::string send_delete_request(const std::string &url);

std::string
send_delete_request(const std::string &url, const nlohmann::json &json_data);

std::string
send_patch_request(const std::string &url, const nlohmann::json &json_data);

#endif  // COMMON_FOR_ALL_HPP_
