#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "handlers.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

class Session;

class Server {
public:
    Server(
        boost::asio::io_context &io_context,
        short port,
        Database_Handler &db_handler
    );

private:
    void accept();

    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
    Database_Handler &db_handler;
};

#endif  // SERVER_HPP_