#ifndef USERS_HPP
#define USERS_HPP

#include <string>
#include <sqlite3.h>

// Структура для хранения данных о пользователе
struct User {
    std::string last_name;   
    std::string first_name;   
    std::string patronymic;   
    std::string role;         
    std::string password_hash;  
    std::string birth_date;    
    std::string email;           
    std::string phone;         
};

class UserDatabase {
public:
    // Открывает базу данных с заданным именем
    UserDatabase(const std::string &db_name);
    
    // Закрывает соединение с базой данных
    ~UserDatabase();

    // Создает таблицу Users, если она еще не существует; иначе выводит предупреждение
    bool createTable();

    // Вставляет нового пользователя в таблицу Users
    bool insertUser(const User &user);

    // Обновляет данные пользователя с заданным id; если его нет, выводит сообщение и отменяет ввод
    bool updateUser(int user_id, const User &user);

    // Удаляет пользователя по заданному id; если его нет, выводит сообщение
    bool deleteUser(int user_id);

    // Проверяет, существует ли пользователь с заданным id; возвращает true, если существует
    bool userExists(int user_id);

private:
    sqlite3 *db;
    // Выполняет заданный SQL-запрос и возвращает true при успехе
    bool executeSQL(const std::string &sql_query);
};

#endif // USERS_HPP
