#include "../include/database.hpp"
#include "../include/handlers/registration.hpp"
#include "../include/handlers/login.hpp"
#include "../include/system_init.hpp"
#include "../include/server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

database_handler* global_db = nullptr;

int main() {
    try {
        std::string connect_info =
            "dbname=postgres user=postgres password=123 host=localhost port=5432";
        database_handler db(connect_info);

        if (!db.connect()) {
            std::cerr << "Failed to connect to PostgreSQL database." << std::endl;
            return 1;
        }

        if (!db.initialize_database()) {
            std::cerr << "Database initialization error\n";
            return 1;
        }

        global_db = &db;

        boost::asio::io_context io_context;
        server server(io_context, 8080, db);

        std::cout << "Server listening on port 8080..." << std::endl;

        io_context.run(); //встроенная функция
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}