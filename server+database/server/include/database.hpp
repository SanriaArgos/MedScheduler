#ifndef DATABASE_HANDLER_HPP_
#define DATABASE_HANDLER_HPP_

#include <libpq-fe.h>
#include <string>

class database_handler {
public:
    explicit database_handler(const std::string &connect_information);
    ~database_handler();

    bool connect();

    bool initialize_database();

    PGconn *get_connection() const;

    PGconn *connection_ = nullptr;
    const std::string connect_information_;

    static std::string prepare_query(const std::string &query);
    static std::string generate_salt(size_t length = 16);
    static std::string
    hash_password(const std::string &password, const std::string &salt);
};

#endif  // DATABASE_HANDLER_HPP_