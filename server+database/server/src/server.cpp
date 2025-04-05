#include "../include/session.hpp"
#include "../include/server.hpp"
#include <functional>
#include <iostream>

server::server(
    boost::asio::io_context &io_context,
    short port,
    database_handler &db_handler
)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      db_handler(db_handler) {
    accept();
}

void server::accept() {
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<session>(std::move(socket), std::ref(db_handler))
                ->start();
        } else {
            std::cerr << "Error accepting connection: " << ec.message()
                      << std::endl;
        }
        accept();
    });
}
