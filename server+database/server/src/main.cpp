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
        const std::string superuser_connect_info = "dbname=postgres user=postgres password=123 host=localhost port=5432";
        if (!initialize_system(superuser_connect_info)) {
            std::cerr << "System initialization error\n";
            return 1;
        }
        
        const std::string connect_information = "dbname=medscheduler user=meduser password=3671920119 host=localhost port=5432";
        database_handler db(connect_information);
        if (!db.connect()) {
            std::cerr << "Database connection error\n";
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

        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
