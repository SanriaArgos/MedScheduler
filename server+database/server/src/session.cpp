#include "../include/session.hpp"
#include "../include/cors_handler.hpp"
#include "../include/handler_request.hpp"
#include <iostream>

session::session(tcp::socket socket, database_handler &db_handler)
    : socket_(std::move(socket)), db_handler(db_handler) {}

void session::start() {
    read_request();
}

void session::read_request() {
    auto self = shared_from_this();
    http::async_read(
        socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                self->process_request();
            } else {
                std::cerr << "Error reading request: " << ec.message() << std::endl;
            }
        });
}

void session::process_request() {
    // Специальная обработка для preflight OPTIONS запросов
    if (request_.method() == http::verb::options) {
        send_options_response();
        return;
    }

    response_.version(request_.version());
    response_.keep_alive(request_.keep_alive());

    // Обработка запроса
    handle_request(request_, response_, db_handler);
    
    // Добавляем CORS-заголовки ко всем ответам
    add_cors_headers(response_);
    
    send_response();
}

void session::send_options_response() {
    auto self = shared_from_this();
    auto response = create_options_response(request_);
    
    http::async_write(
        socket_, response,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (ec) {
                std::cerr << "Error sending OPTIONS response: " << ec.message() << std::endl;
            }
            self->socket_.shutdown(tcp::socket::shutdown_send, ec);
        });
}

void session::send_response() {
    auto self = shared_from_this();
    http::async_write(
        socket_, response_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                if (self->response_.keep_alive()) {
                    self->request_ = {};
                    self->response_ = {};
                    self->buffer_.consume(self->buffer_.size());
                    self->read_request();
                } else {
                    self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                }
            } else {
                std::cerr << "Error writing response: " << ec.message() << std::endl;
            }
        });
}
