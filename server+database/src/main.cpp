#include "handlers.hpp"
#include "server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <string>

std::mutex mutex;

int main() {
    try {
        std::string connect_info =
            "dbname=medscheduler user=meduser password=123 host=localhost "
            "port=5432";
        Database_Handler db_handler(connect_info);

        if (!db_handler.connect()) {
            std::cerr << "Failed to connect to PostgreSQL database."
                      << std::endl;
            return EXIT_FAILURE;
        }

        boost::asio::io_context io_context;
        Server server(io_context, 8080, db_handler);

        std::cout << "Server listening on port 8080..." << std::endl;

        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}