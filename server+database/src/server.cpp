#include "session.hpp"
#include "server.hpp"
#include <functional>
#include <iostream>

Server::Server(
    boost::asio::io_context &io_context,
    short port,
    Database_Handler &db_handler
)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      db_handler(db_handler) {
    accept();
}

void Server::accept() {
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<Session>(std::move(socket), std::ref(db_handler))
                ->start();
        } else {
            std::cerr << "Error accepting connection: " << ec.message()
                      << std::endl;
        }
        accept();
    });
}