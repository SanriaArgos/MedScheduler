#include "system_init.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <sstream>
#include <string>

static bool execute_sql(PGconn *conn, const std::string &sql) {
    PGresult *res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "SQL error: " << PQerrorMessage(conn) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

static bool check_user_exists(PGconn *conn, const std::string &username) {
    std::stringstream ss;
    ss << "SELECT 1 FROM pg_catalog.pg_user WHERE usename = '" << username
       << "'";
    PGresult *res = PQexec(conn, ss.str().c_str());
    bool exists =
        (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

static bool check_database_exists(PGconn *conn, const std::string &dbname) {
    std::stringstream ss;
    ss << "SELECT 1 FROM pg_database WHERE datname = '" << dbname << "'";
    PGresult *res = PQexec(conn, ss.str().c_str());
    bool exists =
        (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

bool initialize_system(const std::string &superuser_connect_info) {
    PGconn *conn = PQconnectdb(superuser_connect_info.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Ошибка подключения (superuser): " << PQerrorMessage(conn)
                  << "\n";
        PQfinish(conn);
        return false;
    }
    if (!check_user_exists(conn, "meduser")) {
        std::string create_user_sql =
            "CREATE USER meduser WITH PASSWORD '3671920119'";
        if (!execute_sql(conn, create_user_sql)) {
            std::cerr << "Не удалось создать пользователя meduser\n";
            PQfinish(conn);
            return false;
        }
        std::cout << "Пользователь meduser создан\n";
    } else {
        std::cout << "Пользователь meduser уже существует\n";
    }
    if (!check_database_exists(conn, "medscheduler")) {
        std::string create_db_sql =
            "CREATE DATABASE medscheduler WITH OWNER = meduser";
        if (!execute_sql(conn, create_db_sql)) {
            std::cerr << "Не удалось создать базу данных medscheduler\n";
            PQfinish(conn);
            return false;
        }
        std::cout << "БД medscheduler создана\n";
    } else {
        std::cout << "БД medscheduler уже существует\n";
    }
    PQfinish(conn);
    return true;
}
