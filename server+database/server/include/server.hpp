#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "database.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

class session;

class server {
public:
    server(
        boost::asio::io_context &io_context,
        short port,
        database_handler &db_handler
    );
    void run_server();
    
private:
    void accept();

    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
    database_handler &db_handler;
};

#endif  // SERVER_HPP_