#ifndef DATABASE_HANDLER_HPP_
#define DATABASE_HANDLER_HPP_

#include <libpq-fe.h>
#include <string>


// базовый класс для работы с базой данных
class database_handler {
public:
    explicit database_handler(const std::string &connect_information);
    ~database_handler();

    bool connect();
    bool user_exists(const std::string &phone) const;
    bool register_user(
        const std::string &last_name,
        const std::string &first_name,
        const std::string &patronymic,
        const std::string &phone,
        const std::string &password
    );
    std::string
    login_user(const std::string &phone, const std::string &password) const;
    std::string get_patient_records(int patient_id) const;
    int get_user_id_by_phone(const std::string &phone) const;
    std::string get_user_type_by_phone(const std::string &phone) const;

    // Функция инициализации базы данных (DDL)
    bool initialize_database();

    PGconn *get_connection() const;

private:
    PGconn *connection_ = nullptr;
    const std::string connect_information_;

    static std::string prepare_query(const std::string &query);
    static std::string generate_salt(size_t length = 16);
    static std::string
    hash_password(const std::string &password, const std::string &salt);
};

#endif  // DATABASE_HANDLER_HPP_