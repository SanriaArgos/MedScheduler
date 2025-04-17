#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context io_context; //создаем обработчик операций
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
    //создаем один конец для приема - сервер
    std::cout << "Listening at " << acceptor.local_endpoint() << "\n";
    //говорим, что сервер - local_endpoint - слушает

    while (true) {
        tcp::socket s = acceptor.accept(); 
        //приниматель - прими - ждем, пока клиент подключится
        //получили сокет - передаем потоку
        // и дальше уже сообщения всякие
        std::thread([s = std::move(s)]() mutable {
            std::cout << "Accepted connection " << s.remote_endpoint()
                      << " --> " << s.local_endpoint() << "\n";
            tcp::iostream client(std::move(s));

            while (client) {
                std::string s;
                if (!(client >> s)) {
                    break;
                }
                client << "got from you: " << s << "\n";
            }
            std::cout << "Completed\n";
        }).detach();
        //здесь создали поток и отправляем его в свободное плвание 
        //то есть он может существовать и работать даже после окончания мейна
    }
}

//важно!!!
//здесь мы создаем клиентов и эхование не зависит от других клиентов - получили сообщение - отправляем его сразу назад
//это происходит потому, что мы сделали потоки
