#include "handlers.hpp"
#include <openssl/sha.h>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

Database_Handler::Database_Handler(const std::string &connect_information)
    : connect_information_(connect_information) {
}

Database_Handler::~Database_Handler() {
    if (connection_) {
        PQfinish(connection_);
    }
}

bool Database_Handler::connect() {
    connection_ = PQconnectdb(connect_information_.c_str());
    return PQstatus(connection_) == CONNECTION_OK;
}

std::string Database_Handler::prepare_Query(const std::string &query) {
    std::string cleaned = query;
    size_t position = 0;
    while ((position = cleaned.find("'", position)) != std::string::npos) {
        cleaned.replace(position, 1, "''");
        position += 2;
    }
    return cleaned;
}

std::string Database_Handler::generate_Salt(size_t length) {
    static const char char_set[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist(0, (int)(sizeof(char_set) - 2));
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt.push_back(char_set[dist(engine)]);
    }
    return salt;
}

std::string Database_Handler::hash_Password(
    const std::string &password,
    const std::string &salt
) {
    std::string to_hash = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
    SHA256(
        reinterpret_cast<const unsigned char *>(to_hash.c_str()),
        to_hash.size(), hash
    );
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool Database_Handler::user_Exists(const std::string &phone) {
    std::string query = "SELECT 1 FROM users WHERE phone = '" +
                        prepare_Query(phone) + "' AND user_type = 'patient'";
    PGresult *res = PQexec(connection_, query.c_str());
    bool exists =
        (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

bool Database_Handler::register_User(
    const std::string &last_name,
    const std::string &first_name,
    const std::string &patronymic,
    const std::string &phone,
    const std::string &password
) {
    std::string salt = generate_Salt(16);
    std::string hashed_pass = hash_Password(password, salt);
    std::string query =
        "INSERT INTO users (last_name, first_name, patronymic, phone, "
        "hashed_password, salt, user_type) "
        "VALUES ('" +
        prepare_Query(last_name) + "', '" + prepare_Query(first_name) + "', '" +
        prepare_Query(patronymic) + "', '" + prepare_Query(phone) + "', '" +
        prepare_Query(hashed_pass) + "', '" + prepare_Query(salt) +
        "', 'patient')";
    PGresult *res = PQexec(connection_, query.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return success;
}

std::string Database_Handler::login_User(
    const std::string &phone,
    const std::string &password
) {
    std::string query =
        "SELECT id, user_type, hashed_password, salt FROM users WHERE phone = "
        "'" +
        prepare_Query(phone) + "'";
    PGresult *res = PQexec(connection_, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (res) {
            PQclear(res);
        }
        return "";
    }
    std::string user_id = PQgetvalue(res, 0, 0);
    std::string user_type = PQgetvalue(res, 0, 1);
    std::string db_hash = PQgetvalue(res, 0, 2);
    std::string db_salt = PQgetvalue(res, 0, 3);
    PQclear(res);
    std::string attempt_hash = hash_Password(password, db_salt);
    if (attempt_hash != db_hash) {
        return "";
    }
    if (user_type == "patient") {
        return "patient:" + user_id;
    }
    if (user_type == "главный администратор") {
        return "superadmin:" + user_id;
    }
    return "success";
}
