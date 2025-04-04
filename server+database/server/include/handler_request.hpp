#ifndef HANDLERS_REQUEST_HPP_
#define HANDLERS_REQUEST_HPP_

#include "../include/database.hpp"
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp> // Для split и decode base64

namespace beast = boost::beast;
namespace http = beast::http;
using json = nlohmann::json;

std::string base64_decode(const std::string &encoded);

bool extract_basic_credentials(const std::string &auth_header, std::string &phone, std::string &password);

// void handle_login(const json &body, http::response<http::string_body> &res, database_handler &db_handler);

// void handle_registration(const json &body, http::response<http::string_body> &res, database_handler &db_handler);

void handle_not_found(http::response<http::string_body> &res);

void handle_error(const std::exception &e, http::response<http::string_body> &res);

void handle_request(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    database_handler &db_handler
);

#endif  // HANDLERS_REQUEST_HPP_