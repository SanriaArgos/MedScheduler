#include "../include/database.hpp"
#include <openssl/sha.h>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

database_handler::database_handler(const std::string &connect_information)
    : connect_information_(connect_information) {
}

database_handler::~database_handler() {
    if (connection_ != nullptr) {
        PQfinish(connection_);
    }
}

bool database_handler::connect() {
    connection_ = PQconnectdb(connect_information_.c_str());
    return (PQstatus(connection_) == CONNECTION_OK);
}

std::string database_handler::prepare_query(const std::string &query) {
    std::string cleaned = query;
    size_t position = 0;
    while ((position = cleaned.find("'", position)) != std::string::npos) {
        cleaned.replace(position, 1, "''");
        position += 2;
    }
    return cleaned;
}

std::string database_handler::generate_salt(size_t length) {
    static const char char_set[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist(
        0, static_cast<int>(sizeof(char_set) - 2)
    );
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt.push_back(
            static_cast<char>(char_set[static_cast<size_t>(dist(engine))])
        );
    }
    return salt;
}

std::string database_handler::hash_password(
    const std::string &password,
    const std::string &salt
) {
    std::string to_hash = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
    SHA256(
        reinterpret_cast<const unsigned char *>(to_hash.c_str()),
        static_cast<size_t>(to_hash.size()), hash
    );
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool database_handler::initialize_database() {
    std::ifstream file("create_tables.sql");
    if (!file.is_open()) {
        std::cerr << "не удалось открыть create_tables.sql\n";
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string sql = buffer.str();
    PGresult *res = PQexec(connection_, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "ошибка инициализации базы: "
                  << PQerrorMessage(connection_) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

PGconn *database_handler::get_connection() const {
    return connection_;
}