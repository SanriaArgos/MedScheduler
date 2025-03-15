#include "session.hpp"
#include "server.hpp"
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

void server::run_server() {
    try {
        std::string connect_info =
            "dbname=postgres user=postgres password=123 host=localhost port=5432";
        database_handler db_handler(connect_info);

        if (!db_handler.connect()) {
            std::cerr << "Failed to connect to PostgreSQL database." << std::endl;
            return;
        }

        boost::asio::io_context io_context;
        server server(io_context, 8080, db_handler);

        std::cout << "Server listening on port 8080..." << std::endl;

        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}