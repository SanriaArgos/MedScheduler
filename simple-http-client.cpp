#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/filesystem.hpp>
#include <boost/beast.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

void fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

//класс для управления жизненным циклом запроса
class session : public std::enable_shared_from_this<session> {
    tcp::resolver resolver_; //при помощи этой штуки будем искать IP-адрес сервера по домену - имени сайта, который передали
    //без него мы бы не смогли получить ответ от нужного сервера, так как знаем только его имя
    //если бы передавали не имя, а IP-адрес сервера, то резолвер не нужен был бы
    ssl::stream<beast::tcp_stream> stream_; //поток для связи с сервером, теперь защищенный
    beast::flat_buffer buffer_; //буфер для хранения входящих данных - результата запроса // (Must persist between reads)
    http::request<http::empty_body> req_; //объект http-запроса
    http::response<http::string_body> res_; //объект http-ответа
    ssl::context& ctx_;

public:
    //передаем поток, в который будет выводиться результат
    //почему в сессии нужно инициализировать только два поля при помощи ioc?

    //условный планировщик всех действий - он решает когда какой запрос обработать
    explicit session(net::io_context& ioc, ssl::context& ctx)
        : resolver_(net::make_strand(ioc)) //резолверу нужен ioc, чтобы понимать, какой домен дальше нужно искать
        , stream_(net::make_strand(ioc), ctx), ctx_(ctx)  {}
        //strand - штука, при помощи которой
        //предотвращаем гонки данных: то есть у нас один поток ioc, запросы ассинхронные,
        //поэтому в один момент в поток могут записываться ответы РАЗНЫХ запросов
        //net::make_strand помогает от этого избавиться
        //в общем, обуспечиваем потокобезопасность

    // Start the asynchronous operation
    void run(char const* host, char const* port, char const* target, int version){

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream_.native_handle(), host))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            std::cerr << ec.message() << "\n";
            return;
        }

        //target - путь запроса
        //Set up an HTTP GET request message
        //есть несколько видов http-запросов, но самый актуальный сейчас для нас - get - получить данные
        req_.version(version); //устанавливаем используемую версию http
        req_.method(http::verb::get); 
        req_.target(target); //указываем путь-целей к ресурсу
        //например в лмс есть несколько целей для выбора: зачетка, запись на вкр, заявка на стипендию и тд.
        req_.set(http::field::host, host); //www.example.com - пример
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        //сервер должен строго понимать, от кого идет запрос
        //поэтому тут автоматически говорим, что это библиотека
        //можем передать любые данные, то есть, например, какие-то данные клиента

        // Look up the domain name
        resolver_.async_resolve(host, port, beast::bind_front_handler(&session::on_resolve, shared_from_this()));
        //beast::bind_front_handler связывает объект текущей сессии, с методом
        //отправляем запрос на сервер, сохраняя асинхронность
    }

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
        if(ec) //ошибка возникает, если прошлый метод не нашел переданный сервер, то есть не преобразовал его в IP или еще что-то
            return fail(ec, "resolve");

        // Set a timeout on the operation
         beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
        //если не получается обработать операцию в течение 30 сек - протухаем

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(stream_).async_connect(results, beast::bind_front_handler(&session::on_connect, shared_from_this()));
        //хотим установить асинхронный контакт
        //передаем results, чтобы по результатам действия понимать каков ответ
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type){
        if(ec) //если on_resolve не смог становить соединение, то падаем - нам такое не надо
            return fail(ec, "connect");

        // // снова устанавливаем лимит на выполнение операции
        //beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        stream_.async_handshake(ssl::stream_base::client, beast::bind_front_handler(&session::on_handshake, shared_from_this())); //шифруемся
    }

    void on_handshake(beast::error_code ec) {
        if (ec) {
            return fail(ec, "handshake");
        }

        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
        // Отправляем HTTP-запрос на сервер.
        // Если запрос прошел успешно, будет вызвана функция on_write для дальнейшей обработки.
        http::async_write(stream_, req_, beast::bind_front_handler(&session::on_write, shared_from_this()));
    }

    void on_write(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if(ec) //опять же - если не получилось что-то на предыдущем шаге - падаем
            return fail(ec, "write");
        
        // Receive the HTTP response. да, хоти получить-считать ответ от сервера
        http::async_read(stream_, buffer_, res_, beast::bind_front_handler(&session::on_read, shared_from_this()));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred){
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");

        // Write the message to standard out
        std::cout << res_ << std::endl;
        // Выводим на стандартный вывод HTTP-ответ, включая код ответа, тип и тело ответа.

        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Gracefully close the socket
        stream_.async_shutdown(beast::bind_front_handler(&session::on_shutdown, shared_from_this()));
        //успешно выполнили запрос - успешно прибиваем соединение 
        //с двух сторон - со стороны севрера и со стороны клиента

        // not_connected hap pens sometimes so don't bother reporting it.
        if(ec && ec != beast::errc::not_connected)
            return fail(ec, "shutdown");
        // If we get here then the connection is closed gracefully
    }

    void on_shutdown(beast::error_code ec)
    {
        if(ec == net::error::eof) {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }
        if(ec) {
            return fail(ec, "shutdown");
        }

        // If we get here then the connection is closed gracefully
    }
};


int main(int argc, char** argv){
    auto const host = argv[1];
    auto const port = argv[2];
    auto const target = argv[3];
    int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

    //условный планировщик всех действий - он решает когда какой запрос обработать
    net::io_context ioc;

    boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);
    // подгружаем сертификаты
    // ctx.use_certificate_file("certificate.pem", boost::asio::ssl::context::pem);
    // ctx.use_private_key_file("private_key.pem", boost::asio::ssl::context::pem);

    std::shared_ptr<session> s = std::make_shared<session>(ioc, ctx);

    // Launch the asynchronous operation
    s->run(host, port, target, version);
    //здесь передаем IP-адрес клиента, порт, по которому хотим подключитьсz
    //цель - сайт, и версия http - опционально, но обычно используют 1.1

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    return EXIT_SUCCESS;
}

//g++ -std=c++17 -o http_request_program http_request_program.cpp -lboost_system -lboost_filesystem -lboost_thread -pthread
//./a.out jsonplaceholder.typicode.com 80 /posts 1.1 - example

//openssl genpkey -algorithm RSA -out private_key.pem
//ключ для подписи сертификата
//openssl req -new -x509 -key private_key.pem -out certificate.pem -days 365
//создание самого сертификата

//сделать больше методов, сделать обработку json
