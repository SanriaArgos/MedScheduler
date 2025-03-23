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
    std::uniform_int_distribution<> dist(0, static_cast<int>(sizeof(char_set) - 2));
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt.push_back(static_cast<char>(char_set[static_cast<size_t>(dist(engine))]));
    }
    return salt;
}

std::string database_handler::hash_password(const std::string &password,
                                              const std::string &salt) {
    std::string to_hash = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
    SHA256(reinterpret_cast<const unsigned char *>(to_hash.c_str()),
           static_cast<size_t>(to_hash.size()), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

// Исправлено: теперь проверяем наличие номера телефона во всей таблице
bool database_handler::user_exists(const std::string &phone) const {
    std::string query = "SELECT 1 FROM users WHERE phone = '" + prepare_query(phone) + "'";
    PGresult *res = PQexec(connection_, query.c_str());
    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

bool database_handler::register_user(const std::string &last_name, const std::string &first_name,
                                       const std::string &patronymic, const std::string &phone,
                                       const std::string &password) {
    std::string salt = generate_salt(16);
    std::string hashed_pass = hash_password(password, salt);
    std::string query =
        "INSERT INTO users (last_name, first_name, patronymic, phone, hashed_password, salt, user_type) "
        "VALUES ('" +
        prepare_query(last_name) + "', '" + prepare_query(first_name) + "', '" +
        prepare_query(patronymic) + "', '" + prepare_query(phone) + "', '" +
        prepare_query(hashed_pass) + "', '" + prepare_query(salt) + "', 'patient')";
    PGresult *res = PQexec(connection_, query.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    if (!success) {
        std::cerr << "Ошибка регистрации: " << PQerrorMessage(connection_) << "\n";
    }
    PQclear(res);
    return success;
}

std::string database_handler::get_patient_records(int patient_id) const {
    std::string query = "SELECT r.time_and_date, h.address FROM records r "
                        "JOIN hospitals h ON r.hospital_id = h.hospital_id "
                        "WHERE r.patient_id = " + std::to_string(patient_id);
    PGresult *res = PQexec(connection_, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        if (res)
            PQclear(res);
        return "";
    }
    std::string result;
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        result += std::string(PQgetvalue(res, i, 0)) + " | " +
                  PQgetvalue(res, i, 1) + "\n";
    }
    PQclear(res);
    return result;
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
        std::cerr << "ошибка инициализации базы: " << PQerrorMessage(connection_) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

int database_handler::get_user_id_by_phone(const std::string &phone) const {
    const char* params[1] = { phone.c_str() };
    PGresult *res = PQexecParams(
        connection_,
        "SELECT id FROM users WHERE phone = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
            std::cerr << "DB error (get_user_id): " << PQerrorMessage(connection_);
        PQclear(res);
        return -1;
    }
    int id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return id;
}

std::string database_handler::get_user_type_by_phone(const std::string &phone) const {
    const char* params[1] = { phone.c_str() };
    PGresult *res = PQexecParams(
        connection_,
        "SELECT user_type FROM users WHERE phone = $1",
        1, nullptr, params, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
            std::cerr << "DB error (get_user_type): " << PQerrorMessage(connection_);
        PQclear(res);
        return "";
    }
    std::string user_type = PQgetvalue(res, 0, 0);
    PQclear(res);
    return user_type;
}

std::string database_handler::login_user(const std::string &phone,
                                           const std::string &password) const {
    const char* paramValues[1] = { phone.c_str() };
    PGresult *res = PQexecParams(connection_,
        "SELECT id, user_type, hashed_password, salt FROM users WHERE phone = $1",
        1,
        NULL,
        paramValues,
        NULL,
        NULL,
        0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (res) PQclear(res);
        return "";
    }
    std::string user_id = PQgetvalue(res, 0, 0);
    std::string user_type = PQgetvalue(res, 0, 1);
    std::string db_hash = PQgetvalue(res, 0, 2);
    std::string db_salt = PQgetvalue(res, 0, 3);
    PQclear(res);
    std::string attempt_hash = hash_password(password, db_salt);
    if (attempt_hash != db_hash) {
        return "";
    }
    if (user_type == "patient") {
        return "patient:" + user_id;
    } else if (user_type == "senior administrator") {
        return "senior:" + user_id;
    } else if (user_type == "junior administrator") {
        return "junior:" + user_id;
    } else if (user_type == "doctor") {
        // Получение doctor_id из таблицы doctors по user_id.
        std::string query2 = "SELECT doctor_id FROM doctors WHERE user_id = " + user_id;
        PGresult *res2 = PQexec(connection_, query2.c_str());
        if (!res2 || PQresultStatus(res2) != PGRES_TUPLES_OK || PQntuples(res2) == 0) {
            if (res2) PQclear(res2);
            return "";
        }
        std::string doctor_id = PQgetvalue(res2, 0, 0);
        PQclear(res2);
        return "doctor:" + doctor_id;
    }
    return "success";
}

PGconn* database_handler::get_connection() const {
    return connection_;
}