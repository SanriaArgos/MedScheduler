// session.hpp
#ifndef SESSION_HPP_
#define SESSION_HPP_

#include "handlers.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    // Конструктор теперь принимает ссылку на Database_Handler
    Session(tcp::socket socket, Database_Handler &db_handler);
    void start();

private:
    tcp::socket socket_;
    Database_Handler &db_handler_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> request_;
    http::response<http::string_body> response_;

    void read_request();
    void process_request();
    void write_response();
};

#endif  // SESSION_HPP_
