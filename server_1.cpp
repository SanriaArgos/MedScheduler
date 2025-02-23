#include <iostream>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context io_context; //штука, которая работает с операциями
    //нужно прописать обзательно - неважно синхронные операции(работа с несколькими клиентами одновремнно) или асинхронные(работа с лиентами последовательна)
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345)); //принимающий объект - слушает соединения по заданному ip-адресу и порту
    std::cout << "Listening at " << acceptor.local_endpoint() << "\n"; //как раз-таки выводит IP-адрес и номер порта для прослушки, то есть то, что мы указали ранее
    //local_endpoint - конечная точка, то есть один из концов соединения
    
    while (true) { //создаем бесконченый цикл для обработки клиентов
        tcp::socket s = acceptor.accept(); //ждем пока клиент подключится к нашему сокету. пока не подключится - будем ждать
        std::cout << "Accepted connection " << s.remote_endpoint() << " --> "
                  << s.local_endpoint() << "\n"; //выводим сокет клиента --> сокет сервера
        tcp::iostream client(std::move(s)); //создаем поток для общения с клиентом, основанный на его сокете
        //обязательно мувать - иначе не прокатит

        while (client) { // пока есть подключение с клиентом 
            std::string s; //хотим получать от него какое-то сообщение
            if (!(client >> s)) { //если клиент ничего не вводит - заканчиваем работу
                break;
            }
            client << "got from you: " << s << "\n"; //выводим, что получили - типа эхо
        }
        std::cout << "Completed\n"; // закончили работу с этим клиентом
    }
}

//чтобы подключиться компилируем файл
//в одном терминале запускаем исполняемый файл
//в другом файле пишем nc 127.0.0.1 12345 - то есть локальный сокет
//в другом должно вывестись, что мы подключились 
//в том, в котором писали nc 127.0.0.1 12345 можем писать сообщения и сервер будет эховать, что написали
//можем подключить другого клиента и даже что-то посылать серверу, но он ответит только тогда, когда закончит работу с другим клиентом
