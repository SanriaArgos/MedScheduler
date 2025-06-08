#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>
#include "../include/database.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket, database_handler &db_handler);
    void start();

private:
    void read_request();
    void process_request();
    void send_response();
    // Удалить строку: void send_options_response();

    tcp::socket socket_;
    beast::flat_buffer buffer_{8192};
    http::request<http::string_body> request_;
    http::response<http::string_body> response_;
    database_handler &db_handler;
};