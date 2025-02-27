#include "users.hpp"

#include <iostream>
#include <sstream>

// Конструктор, открывающий базу данных с указанным именем
UserDatabase::UserDatabase(const std::string &db_name) : db(nullptr) {
    int result_code = sqlite3_open(db_name.c_str(), &db);
    if(result_code) {
        std::cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << std::endl;
    }
}

// Деструктор, закрывающий соединение с базой данных
UserDatabase::~UserDatabase() {
    if(db) {
        sqlite3_close(db);
    }
}

// Выполняет заданный SQL-запрос и возвращает true при успехе
bool UserDatabase::executeSQL(const std::string &sql_query) {
    char *error_msg = nullptr;
    int result_code = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &error_msg);

    if(result_code != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        return false;
    }

    return true;
}

// Проверяет, существует ли пользователь с заданным id; возвращает true, если да
bool UserDatabase::userExists(int user_id) {
    std::ostringstream sql_stream;
    sql_stream << "SELECT COUNT(*) FROM Users WHERE id = " << user_id << ";";
    int user_count = 0;

    auto callback = [](void* data, int argc, char** argv, char** col_names) -> int {
        int* count_ptr = static_cast<int*>(data);
        if (argc > 0 && argv[0]) {
            *count_ptr = std::stoi(argv[0]);
        }
        return 0;
    };

    char* error_msg = nullptr;
    int result_code = sqlite3_exec(db, sql_stream.str().c_str(), callback, &user_count, &error_msg);

    if (result_code != SQLITE_OK) {
        std::cerr << "Ошибка проверки существования пользователя: " << error_msg << "\n";
        sqlite3_free(error_msg);
        return false;
    }

    return user_count > 0;
}

// Создает таблицу Users, если она еще не существует; иначе выводит предупреждение и отменяет создание
bool UserDatabase::createTable() {
    // Проверяем, существует ли таблица Users
    bool table_exists = false;
    std::string check_sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='Users';";
    
    auto callback = [](void* data, int argc, char** argv, char** col_names) -> int {
        bool* exists_ptr = static_cast<bool*>(data);
        if (argc > 0) {
            *exists_ptr = true;
        }
        return 0;
    };

    char *error_msg = nullptr;
    int result_code = sqlite3_exec(db, check_sql.c_str(), callback, &table_exists, &error_msg);
    
    if (result_code != SQLITE_OK) {
        std::cerr << "Ошибка проверки существования таблицы: " << error_msg << "\n";
        sqlite3_free(error_msg);
        return false;
    }

    if (table_exists) {
        std::cerr << "Таблица Users уже существует. Создание таблицы отменено.\n";
        return false;
    }

    // Если таблицы нет, создаем ее
    std::string create_table_sql = R"(
        CREATE TABLE Users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            last_name TEXT NOT NULL CHECK(LENGTH(last_name) <= 50 AND TRIM(last_name) != ''),
            first_name TEXT NOT NULL CHECK(LENGTH(first_name) <= 50 AND TRIM(first_name) != ''),
            patronymic TEXT NOT NULL CHECK(LENGTH(patronymic) <= 50 AND TRIM(patronymic) != ''),
            role TEXT NOT NULL CHECK(role IN ('пациент','врач','администратор') AND LENGTH(role) <= 50 AND TRIM(role) != ''),
            password_hash TEXT NOT NULL CHECK(LENGTH(password_hash) <= 50 AND TRIM(password_hash) != ''),
            birth_date TEXT NOT NULL CHECK(LENGTH(birth_date) <= 50 AND TRIM(birth_date) != ''),
            email TEXT NOT NULL UNIQUE CHECK(LENGTH(email) <= 50 AND TRIM(email) != ''),
            phone TEXT NOT NULL CHECK(LENGTH(phone) <= 50 AND TRIM(phone) != '')
        );
    )";
    return executeSQL(create_table_sql);
}

// Вставляет нового пользователя в таблицу Users
bool UserDatabase::insertUser(const User &user) {
    std::ostringstream sql_stream;
    sql_stream << "INSERT INTO Users (last_name, first_name, patronymic, role, password_hash, birth_date, email, phone) VALUES ("
               << "'" << user.last_name << "', "
               << "'" << user.first_name << "', "
               << "'" << user.patronymic << "', "
               << "'" << user.role << "', "
               << "'" << user.password_hash << "', "
               << "'" << user.birth_date << "', "
               << "'" << user.email << "', "
               << "'" << user.phone << "');";
    return executeSQL(sql_stream.str());
}

// Обновляет данные пользователя с заданным id; если он не существует, выводит ошибку и отменяет ввод
bool UserDatabase::updateUser(int user_id, const User &user) {
    if (!userExists(user_id)) {
        std::cerr << "Ошибка: Пользователь с id = " << user_id << " не существует. Обновление отменено.\n";
        return false;
    }

    std::ostringstream sql_stream;
    sql_stream << "UPDATE Users SET "
               << "last_name = '" << user.last_name << "', "
               << "first_name = '" << user.first_name << "', "
               << "patronymic = '" << user.patronymic << "', "
               << "role = '" << user.role << "', "
               << "password_hash = '" << user.password_hash << "', "
               << "birth_date = '" << user.birth_date << "', "
               << "email = '" << user.email << "', "
               << "phone = '" << user.phone << "' "
               << "WHERE id = " << user_id << ";";
    return executeSQL(sql_stream.str());
}

// Удаляет пользователя с заданным id; если он не существует, выводит ошибку и отменяет удаление
bool UserDatabase::deleteUser(int user_id) {
    if (!userExists(user_id)) {
        std::cerr << "Ошибка: Пользователь с id = " << user_id << " не существует. Удаление отменено.\n";
        return false;
    }
    
    std::ostringstream sql_stream;
    sql_stream << "DELETE FROM Users WHERE id = " << user_id << ";";
    return executeSQL(sql_stream.str());
}
