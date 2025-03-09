#include "database.hpp"
#include <openssl/sha.h>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

/**
 * @brief Конструктор, сохраняющий строку подключения.
 */
Database_Handler::Database_Handler(const std::string &connect_information)
    : connect_information_(connect_information) {
}

/**
 * @brief Деструктор, закрывающий соединение с базой данных, если оно установлено.
 */
Database_Handler::~Database_Handler() {
    if (connection_) {
        PQfinish(connection_);
    }
}

/**
 * @brief Устанавливает соединение с PostgreSQL.
 * @return true, если соединение успешно установлено.
 */
bool Database_Handler::connect() {
    connection_ = PQconnectdb(connect_information_.c_str());
    return PQstatus(connection_) == CONNECTION_OK;
}

/**
 * @brief Подготавливает строку для SQL-запроса, экранируя одинарные кавычки.
 */
std::string Database_Handler::prepare_Query(const std::string &query) {
    std::string cleaned = query;
    size_t position = 0;
    while ((position = cleaned.find("'", position)) != std::string::npos) {
        cleaned.replace(position, 1, "''");
        position += 2; // смещаем позицию на длину заменённой подстроки
    }
    return cleaned;
}

/**
 * @brief Генерирует случайную строку-соль заданной длины.
 */
std::string Database_Handler::generate_Salt(size_t length) {
    static const char char_set[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(sizeof(char_set) - 2));

    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt.push_back(static_cast<char>(char_set[dist(engine)]));
    }
    return salt;
}

/**
 * @brief Хеширует пароль с солью с использованием SHA256.
 */
std::string Database_Handler::hash_Password(const std::string &password,
                                              const std::string &salt) {
    std::string to_hash = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
    SHA256(reinterpret_cast<const unsigned char*>(to_hash.c_str()),
           static_cast<size_t>(to_hash.size()), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

/**
 * @brief Проверяет, существует ли пользователь с указанным номером телефона среди пациентов.
 */
bool Database_Handler::user_Exists(const std::string &phone) {
    std::string query = "SELECT 1 FROM users WHERE phone = '" + prepare_Query(phone) +
                        "' AND user_type = 'patient'";
    PGresult *res = PQexec(connection_, query.c_str());
    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

/**
 * @brief Регистрирует нового пользователя: генерирует соль, хеширует пароль и вставляет данные в таблицу.
 */
bool Database_Handler::register_User(const std::string &last_name,
                                       const std::string &first_name,
                                       const std::string &patronymic,
                                       const std::string &phone,
                                       const std::string &password) {
    std::string salt = generate_Salt(16);
    std::string hashed_pass = hash_Password(password, salt);
    std::string query =
        "INSERT INTO users (last_name, first_name, patronymic, phone, "
        "hashed_password, salt, user_type) "
        "VALUES ('" +
        prepare_Query(last_name) + "', '" + prepare_Query(first_name) + "', '" +
        prepare_Query(patronymic) + "', '" + prepare_Query(phone) + "', '" +
        prepare_Query(hashed_pass) + "', '" + prepare_Query(salt) + "', 'patient')";
    PGresult *res = PQexec(connection_, query.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return success;
}

/**
 * @brief Аутентифицирует пользователя. Сначала ищет пользователя по телефону, затем сравнивает хеш введённого пароля с хешем из базы.
 */
std::string Database_Handler::login_User(const std::string &phone,
                                           const std::string &password) {
    std::string query = "SELECT id, user_type, hashed_password, salt FROM users WHERE phone = '" +
                        prepare_Query(phone) + "'";
    PGresult *res = PQexec(connection_, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (res)
            PQclear(res);
        return "";
    }
    std::string user_id = PQgetvalue(res, 0, 0);
    std::string user_type = PQgetvalue(res, 0, 1);
    std::string db_hashed_pass = PQgetvalue(res, 0, 2);
    std::string db_salt = PQgetvalue(res, 0, 3);
    PQclear(res);
    std::string attempt_hash = hash_Password(password, db_salt);
    if (attempt_hash != db_hashed_pass) {
        return "";
    }
    if (user_type == "patient") {
        return "patient:" + user_id;
    }
    return "success";
}

/**
 * @brief Получает записи пациента, объединяя данные из таблиц records и hospitals.
 */
std::string Database_Handler::get_Patient_Records(int patient_id) {
    std::string query = "SELECT r.time_and_date, h.address FROM records r JOIN hospitals h ON r.hospital_id = h.hospital_id WHERE r.patient_id = " + std::to_string(patient_id);
    PGresult *res = PQexec(connection_, query.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        if (res)
            PQclear(res);
        return "";
    }
    std::string result;
    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        result += std::string(PQgetvalue(res, i, 0)) + " | " + PQgetvalue(res, i, 1) + "\n";
    }
    PQclear(res);
    return result;
}

/**
 * @brief Обновляет личную информацию пользователя.
 * Перед выполнением обновления проверяет, не занят ли новый номер телефона другим пациентом.
 */
bool Database_Handler::update_User_Info(int user_id, const std::string &last_name,
                                          const std::string &first_name,
                                          const std::string &patronymic,
                                          const std::string &phone) {
    std::string query_check = "SELECT id FROM users WHERE phone = '" + prepare_Query(phone) +
                              "' AND user_type = 'patient'";
    PGresult *res_check = PQexec(connection_, query_check.c_str());
    if (PQresultStatus(res_check) == PGRES_TUPLES_OK && PQntuples(res_check) > 0) {
        std::string existing_id = PQgetvalue(res_check, 0, 0);
        if (std::to_string(user_id) != existing_id) {
            PQclear(res_check);
            return false;
        }
    }
    PQclear(res_check);
    std::string query = "UPDATE users SET last_name = '" + prepare_Query(last_name) +
                        "', first_name = '" + prepare_Query(first_name) +
                        "', patronymic = '" + prepare_Query(patronymic) +
                        "', phone = '" + prepare_Query(phone) +
                        "' WHERE id = " + std::to_string(user_id);
    PGresult *res = PQexec(connection_, query.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return success;
}
