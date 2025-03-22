#include "../include/system_init.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <openssl/sha.h>

static bool execute_sql(PGconn *conn, const std::string &sql) {
    PGresult *res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "SQL Error: " << PQerrorMessage(conn) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

static bool check_user_exists(PGconn *conn, const std::string &username) {
    const char* paramValues[1] = { username.c_str() };
    PGresult *res = PQexecParams(conn, "SELECT 1 FROM pg_catalog.pg_user WHERE usename = $1", 1, NULL, paramValues, NULL, NULL, 0);
    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

static bool check_database_exists(PGconn *conn, const std::string &dbname) {
    const char* paramValues[1] = { dbname.c_str() };
    PGresult *res = PQexecParams(conn, "SELECT 1 FROM pg_database WHERE datname = $1", 1, NULL, paramValues, NULL, NULL, 0);
    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}
static std::string generate_salt(size_t length = 16) {
    static const char char_set[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(sizeof(char_set) - 2));
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt.push_back(char_set[dist(engine)]);
    }
    return salt;
}

static std::string hash_password(const std::string &password, const std::string &salt) {
    std::string to_hash = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
    SHA256(reinterpret_cast<const unsigned char*>(to_hash.c_str()), to_hash.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool initialize_system(const std::string &superuser_connect_info) {
    PGconn *conn = PQconnectdb(superuser_connect_info.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Superuser connection error: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn);
        return false;
    }
    if (!check_user_exists(conn, "meduser")) {
        if (!execute_sql(conn, "CREATE USER meduser WITH PASSWORD '3671920119' CREATEDB")) {
            std::cerr << "Failed to create meduser\n";
            PQfinish(conn);
            return false;
        }
        std::cout << "Meduser created\n";
    } else {
        std::cout << "Meduser already exists\n";
    }
    PQfinish(conn);

    const std::string meduser_conn_info = "dbname=postgres user=postgres password=123 host=localhost port=5432";
    PGconn *conn_meduser = PQconnectdb(meduser_conn_info.c_str());
    if (PQstatus(conn_meduser) != CONNECTION_OK) {
        std::cerr << "Meduser connection error: " << PQerrorMessage(conn_meduser) << "\n";
        PQfinish(conn_meduser);
        return false;
    }
    if (!check_database_exists(conn_meduser, "medscheduler")) {
        if (!execute_sql(conn_meduser, "CREATE DATABASE medscheduler")) {
            std::cerr << "Failed to create medscheduler database\n";
            PQfinish(conn_meduser);
            return false;
        }
        std::cout << "Medscheduler database created\n";
    } else {
        std::cout << "Medscheduler database already exists\n";
    }
    PQfinish(conn_meduser);

    const std::string medscheduler_conn_info = "dbname=medscheduler user=postgres password=123 host=localhost port=5432";
    PGconn *conn2 = PQconnectdb(medscheduler_conn_info.c_str());
    if (PQstatus(conn2) != CONNECTION_OK) {
        std::cerr << "Medscheduler connection error (meduser): " << PQerrorMessage(conn2) << "\n";
        PQfinish(conn2);
        return false;
    }
    
    std::ifstream file("create_tables.sql");
    if (!file.is_open()) {
        std::cerr << "Failed to open /create_tables.sql\n";
        PQfinish(conn2);
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string sql = buffer.str();
    PGresult *res_init = PQexec(conn2, sql.c_str());
    if (PQresultStatus(res_init) != PGRES_COMMAND_OK) {
        std::cerr << "Table creation error: " << PQerrorMessage(conn2) << "\n";
        PQclear(res_init);
        PQfinish(conn2);
        return false;
    }
    PQclear(res_init);
    
    // Переустанавливаем владельца таблиц, если они существуют
    execute_sql(conn2, "ALTER TABLE users OWNER TO meduser");
    execute_sql(conn2, "ALTER TABLE hospitals OWNER TO meduser");
    execute_sql(conn2, "ALTER TABLE records OWNER TO meduser");
    execute_sql(conn2, "ALTER TABLE doctors OWNER TO meduser");
    PQfinish(conn2);

    // Подключаемся ещё раз к medscheduler под meduser для создания старшего администратора
    PGconn *conn3 = PQconnectdb(medscheduler_conn_info.c_str());
    if (PQstatus(conn3) != CONNECTION_OK) {
        std::cerr << "Medscheduler connection error (meduser): " << PQerrorMessage(conn3) << "\n";
        PQfinish(conn3);
        return false;
    }
    const char* paramValues[1] = { "senior administrator" };
    PGresult *res = PQexecParams(conn3, "SELECT 1 FROM users WHERE user_type = $1", 1, NULL, paramValues, NULL, NULL, 0);
    bool senior_exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    if (!senior_exists) {
        std::string salt = generate_salt(16);
        std::string hashed_pass = hash_password("1234567890", salt);
        const char* paramValues2[7];
        paramValues2[0] = "";
        paramValues2[1] = "";
        paramValues2[2] = "";
        paramValues2[3] = "+71234567890";
        paramValues2[4] = "senior administrator";
        paramValues2[5] = hashed_pass.c_str();
        paramValues2[6] = salt.c_str();
        PGresult *res_ins = PQexecParams(conn3,
            "INSERT INTO users (last_name, first_name, patronymic, phone, user_type, hashed_password, salt) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)",
            7, NULL, paramValues2, NULL, NULL, 0);
        if (PQresultStatus(res_ins) != PGRES_COMMAND_OK) {
            std::cerr << "Failed to create senior administrator: " << PQerrorMessage(conn3) << "\n";
            PQclear(res_ins);
            PQfinish(conn3);
            return false;
        }
        std::cout << "Senior administrator created\n";
        PQclear(res_ins);
    } else {
        std::cout << "Senior administrator already exists\n";
    }
    PQfinish(conn3);
    return true;
}