#include "example/common/server_certificate.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Return a reasonable mime type based on the extension of a file.
//mime-type - тип, который нужен для понимания, какую информацию мы получили
//обычно он состоит из двух типов
beast::string_view mime_type(beast::string_view path) {
    using beast::iequals;
    auto const ext = [&path]{
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }(); //ниже перечисляем все типы этих самых mime
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(beast::string_view base, beast::string_view path) {
    if(base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

// Return a response for the given request.
// Создаем ответ на принятый запрос
// The concrete type of the response message (which depends on the
// request), is type-erased in message_generator
//по сути нам тут неважно, какой тип сообщения мы получили 
//будем считать, что это абстракция
template <class Body, class Allocator>
http::message_generator handle_request(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req){
    //ниже будут описаны несколько возможных проблем и поведение сервера в этих случаях
    // Returns a bad request response, то есть код 400 Bad Request
    auto const bad_request = [&req](beast::string_view why) { //why - причина ошибки, потом нужна для формирования ответа
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        //говорим, что тип ответа - строка, статус - плохой запрос - 400 и указываем версию http протокола, что и у запроса
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        //в ответе будет указана информация о сервере, автоматически устанавливаем эту магическую строку, которая вернет версию библиотеки
        res.set(http::field::content_type, "text/html");
        //я так понимаю, что здесь мы будем отображать страничку с надписью not_found в случае плохого запроса, поэтому указываем text/html
        res.keep_alive(req.keep_alive());
        //здесь если клиент запросил постоянное соединение, то сохраняем его, если нет - закрываем после ответа
        res.body() = std::string(why);
        //здесь как раз говорим, почему запрос был плохим - непосредственно тело ответа
        res.prepare_payload();
        //готовим тело ответа перед отправкой: вычисляем заголовой и его размер(подобное было в клиенте)
        return res;
    };

    // Returns a not found response
    //говорим, что запращиваемые данные не были найдены
    auto const not_found = [&req](beast::string_view target) { //указываем директорию, в которой произошел сбой
        http::response<http::string_body> res{http::status::not_found, req.version()};
        //говорим, что тип ответ на запрос - строка, статус - номер - не найдено,
        //говорим, что версия https, по которой будет будет возвращаться ответ совпадает с версией, по которой создавался запрос
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        //указываем данные о сервере
        res.set(http::field::content_type, "text/html");
        //говорим, что возвращаем обычный html текст
        res.keep_alive(req.keep_alive());
        //если пользователь создает постоянное соединением, то оставляем его, иначе - закрываем после запроса
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        //говорим, на каком конкретно уровне, какая директория была не найдена
        res.prepare_payload();
        //подготавливаем тело ответа - вычисляем заголовок и его длину
        return res;
    };

    //Returns a server error response
    //обрабатываем ошибки, связанные с работой сервера
    auto const server_error = [&req](beast::string_view what) { //говорим причину ошибки сервера
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    //Make sure we can handle the method
    //проверяем, что запрос хочет выполнить разрешенные операции - запрос данных с их возвратом и без возврата
    //ничего удалять не хотим и принмать на хранение не хотим
    if(req.method() != http::verb::get && req.method() != http::verb::head)
        return bad_request("Unknown HTTP-method");

    //Request path must be absolute and not contain "..".
    //должен быть четко указан путь к запрашиваемому ресурсу - если пусто, то выросла капуста
    //.. означают попытку выйти за корневую директорию - похоже на попытку взлома
    //req.target().find("..") != beast::string_view::npos - проверка на то, что таких символов нет
    if(req.target().empty() || req.target()[0] != '/' || req.target().find("..") != beast::string_view::npos)
        return bad_request("Illegal request-target");

    // Build the path to the requested file
    //создаем абсолютный путь до запрашиваемого файла
    //знаем doc_root - переданный в запросе
    //знаем цель, которую как раз-таки хотим найти в doc_root
    //при помощи path_cat, напоминаю cat в консоли - посмотреть, что написать
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/') //если путь заканчивается /, то это значит, что дошли до самого низа директорий и уже должны оканчиваться index.html - других вариантов нет
        path.append("index.html");

    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec); //пытаемся открыть файл, который запрашивали
    //указываем, что хотим его парсить - scan 
    //и еще может возникнуть ошибка, поэтому передаем ec - куда и запишем ее в случае возникновения

    // Handle the case where the file doesn't exist
    //если ручка указывает на то, что файла не существует, говорим об этом
    if (ec == beast::errc::no_such_file_or_directory)
        return not_found(req.target());

    // Handle an unknown error
    //если ручка вернула что-то странное, сваливаем все беды на сервер - возвращем ошибку
    if (ec)
        return server_error(ec.message());

    // Cache the size since we need it after the move
    //сохраним размер в кеше, потому что запрос может повториться 
    //и мы не хотим потом обрабатывать все заново - хотим быстро вернуть
    auto const size = body.size();

    // Respond to HEAD request
    //если запрос на получение ресурса, но без тела ответа, то есть только заголовки
    //то есть проверям, что ресурс вообще доступен, обновился ли он
    if (req.method() == http::verb::head) {
        http::response<http::empty_body> res{http::status::ok, req.version()};
        //все проверки выше прошли, значит, смело можем установить статус ок
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        //размер контента = размер считанного файла
        res.keep_alive(req.keep_alive());
        return res;
    }

    // Respond to GET request
    //раз запрос не HEAD, значит, GET и уже должны что-то возвращать - возврщать будем в файле
    http::response<http::file_body> res{std::piecewise_construct, std::make_tuple(std::move(body)), std::make_tuple(http::status::ok, req.version())};
    //std::piecewise_construct - говорим, что у нас кусочно-заданная функция - состоит из двух tuple
    //муваем файл для обработки и указываем заголовки как обычно
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, mime_type(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return res;
}

//------------------------------------------------------------------------------

// Report a failure
void fail(beast::error_code ec, char const* what) {
    // ssl::error::stream_truncated, also known as an SSL "short read",
    // indicates the peer closed the connection without performing the
    // required closing handshake (for example, Google does this to
    // improve performance). Generally this can be a security issue,
    // but if your communication protocol is self-terminated (as
    // it is with both HTTP and WebSocket) then you may simply
    // ignore the lack of close_notify.
    //
    // https://github.com/boostorg/beast/issues/38
    //
    // https://security.stackexchange.com/questions/91435/how-to-handle-a-malicious-ssl-tls-shutdown
    //
    // When a short read would cut off the end of an HTTP message,
    // Beast returns the error beast::http::error::partial_message.
    // Therefore, if we see a short read here, it has occurred
    // after the message has been completed, so it is safe to ignore it.

    if (ec == net::ssl::error::stream_truncated)
    //это незначительная ошибка - например, браузер мог просто разорвать соединение без завершения tls-сессии
    //что такое tls-сессия? это когда клиент и сервер договариваются о шифровании данных, обмениваются ключами, передаваемые данные шифруют, после окончания сессии ключи удаляютсяо
        return;

    std::cerr << what << ": " << ec.message() << "\n";
}

//Handles an HTTP server connection
//собстввенно создаем класс для нашей сессии - соединения с сервером 
class session : public std::enable_shared_from_this<session> {
    beast::ssl_stream<beast::tcp_stream> stream_; //мы же шифруемся? надо сделать для этого поток
    beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_; //создаем указатель на место в памяти, где хранится doc_root, чтобы быстро к нему обращаться
    http::request<http::string_body> req_; //ответ - строка

public:
    // Take ownership of the socket
    //создаем соединение с сокетом
    explicit session(
        tcp::socket&& socket,
        ssl::context& ctx,
        std::shared_ptr<std::string const> const& doc_root)
        : stream_(std::move(socket), ctx) //для потока, через который все будет совершаться нужно знать сокет и ssl
        , doc_root_(doc_root) //не забываем про то, откуда будем доставать файлы
    {
    }

    // Start the asynchronous operation
    void run() {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch( //отправка в сеть
            stream_.get_executor(),
            beast::bind_front_handler(&session::on_run, shared_from_this()));
    }

    void on_run() {
        // Set the timeout.
        //здесь извлекаем основной поток из стрима, то есть beast::ssl_stream<beast::tcp_stream>, по которому передаются данные
        beast::get_lowest_layer(stream_).expires_after(
            std::chrono::seconds(30));
        //таймаут нужен, чтобы ожидание не превратилось в вечность

        // Perform the SSL handshake
        //нужно сделать рукопожатие - зашифровать данные
        //bind_front_handler помогает на выходе/входе шифровать/расшифровывать данные
        stream_.async_handshake(
            ssl::stream_base::server,
            beast::bind_front_handler(
                &session::on_handshake, //говорим, что сейчас наш запрос совершает рукопжатие-шифрование
                shared_from_this()));
    }

    void on_handshake(beast::error_code ec) {
        if (ec)
            return fail(ec, "handshake");

        do_read(); //если успешно зашифровались, то можно переходить к считыванию необходимых данных на сервере
    }

    void do_read() { //хотим читать из найденного файла
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
        //здесь возвращаем уже tcp_stream без ssl шифрования

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this())); //из стрима читаем данные, в буфер кладем промежуточные данные, в рек записываем обработанный запрос
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if (ec == http::error::end_of_stream)
            return do_close(); //если сервер понял, что клиент закрыд соединение, то серверу тоже нужно закрыть его

        if (ec)
            return fail(ec, "read");

        send_response(handle_request(*doc_root_, std::move(req_)));
        //отправляем ответ, так как запрос обработан
    }

    void send_response(http::message_generator&& msg) {
        bool keep_alive = msg.keep_alive();

        // Write the response
        beast::async_write(
            stream_,
            std::move(msg),
            beast::bind_front_handler(
                &session::on_write,
                this->shared_from_this(),
                keep_alive)); //в общем хотим вывести ответ
    }

    void on_write(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "write");

        if (!keep_alive) {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // Read another request
        do_read(); // с этим запросом все сделали - читаем следующий
    }

    void do_close() {
        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Perform the SSL shutdown
        stream_.async_shutdown(
            beast::bind_front_handler(&session::on_shutdown, shared_from_this()));
    }

    void on_shutdown(beast::error_code ec) {
        if (ec)
            return fail(ec, "shutdown");
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
//принимаем входящее соединение и заупскаем сессию запроса
class listener : public std::enable_shared_from_this<listener> {
    net::io_context& ioc_; //поток ввода вывода
    ssl::context& ctx_; //должны понимать, как все шифруется
    tcp::acceptor acceptor_; //нужен приниматель запросов
    std::shared_ptr<std::string const> doc_root_; //указываем на место, где хранится необходимый ресурс

public:
    listener(net::io_context& ioc,ssl::context& ctx,tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root)
        : ioc_(ioc), ctx_(ctx), acceptor_(ioc), doc_root_(doc_root) {
        beast::error_code ec;

        // открываем принимателя, узнаем по какому протоколу это было произведено
        acceptor_.open(endpoint.protocol(), ec);
        if (ec) {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        //у нас может быть несколько запросов примерно в одно время, а сокету на сервере нужно очищаться
        //на это нужно время, а запросы не ждут, поэтому здесь говорим, что мы разрешаем пользоваться сокетом до его очистки
        if (ec) {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        //связываем запрос с адресом сервера
        acceptor_.bind(endpoint, ec);
        if (ec) {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        //начинаем слушать соединение, то есть принимать все данные из запроса
        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void run() {
        do_accept();
    }

private:
    void do_accept()
    {
        // The new connection gets its own strand
        //каждое соединение должно быть в своей "ветке" ioc, чтобы не было конфликтов
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
        if (ec) {
            fail(ec, "accept");
            return; // To avoid infinite loop
        }
        else {
            // Create the session and run it
            std::make_shared<session>(std::move(socket), ctx_, doc_root_)->run();
        }

        //создали новую сессию-запрос, на месте не стоим - идем дальше

        // Accept another connection
        do_accept();
    }
};

//------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    // Check command line arguments.
    if (argc != 5) { //ну тут просто проверяем на правильность ввода
        std::cerr << "Usage: http-server-async-ssl <address> <port> <doc_root> <threads>\n" << "Example:\n" << "    http-server-async-ssl 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE; //если что-то пошло не так, то программа падает
    }
    auto const address = net::ip::make_address(argv[1]); //здесь создаем ip-адрес клиента, для того, чтобы понимать, куда потом передавать данные
    auto const port = static_cast<unsigned short>(std::atoi(argv[2])); //также необходимо понимать, на какой порт передавать данные
    auto const doc_root = std::make_shared<std::string>(argv[3]); // Путь к корневой директории документов сервера (где лежат файлы для раздачи)
    auto const threads = std::max<int>(1, std::atoi(argv[4])); //обязательно должен быть поток, в котором мы работаем, 
    //по сути, здесь если передали какую-то фигню, то по умолчанию поток будет один

    // The io_context is required for all I/O
    //как и в клиенте, нам нужен универсальный поток для работы с асинхронным вводом и выводом
    net::io_context ioc{threads};

    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv12}; //тут говорим, что соединение будет шифроваться по ssl/tls и говорим, что версия 1.2

    // This holds the self-signed certificate used by the server
    //для использования ssl нужен сертификат - тут его и загружаем
    load_server_certificate(ctx);

    // Create and launch a listening port
    //создаем слушающий порт на сервере: для его инициализации нужен поток ввода/вывода, ssl благодаря которому как раз через порт будут шифроваться данные
    //говорим, что один конец - endpoint - наш адрес и порт, так как запросу нужно знать два конца, я не знаю, что такое doc_root
    //listener - объект, который принимает соединения и передает их в обработку
    std::make_shared<listener>(ioc, ctx, tcp::endpoint{address, port}, doc_root)->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) {
        v.emplace_back([&ioc]{ioc.run();}); //создаем лямбда-функцию, которая будет создавать поток и класть в вектор потоков, причем сразу происходит запуск потока ввода/вывода
    }
    ioc.run();

    return EXIT_SUCCESS; //если программа выполнилась, то возвращаем успех
}
