#ifndef DATABASE_HANDLER_HPP_
#define DATABASE_HANDLER_HPP_

#include <libpq-fe.h>
#include <string>

class Database_Handler {
public:
    explicit Database_Handler(const std::string &connect_information);
    ~Database_Handler();

    bool connect();

    bool user_Exists(const std::string &phone);
    bool register_User(
        const std::string &last_name,
        const std::string &first_name,
        const std::string &patronymic,
        const std::string &phone,
        const std::string &password
    );
    std::string
    login_User(const std::string &phone, const std::string &password);
    std::string get_Patient_Records(int patient_id);
    bool update_User_Info(
        int user_id,
        const std::string &last_name,
        const std::string &first_name,
        const std::string &patronymic,
        const std::string &phone
    );

    // Методы для главного администратора
    bool mainAdminExists();
    bool createMainAdmin(
        const std::string &last_name,
        const std::string &first_name,
        const std::string &patronymic,
        const std::string &phone,
        const std::string &password
    );
    int getUserIdByPhone(const std::string &phone);
    std::string getUserType(int user_id);
    bool
    setUserTypeByPhone(const std::string &phone, const std::string &new_type);
    bool deleteUser(int user_id);
    bool deleteHospitalsWhereAdmin(int user_id);
    bool insertHospital(
        const std::string &region,
        const std::string &settlement_type,
        const std::string &settlement_name,
        const std::string &street,
        const std::string &house,
        const std::string &full_name,
        int admin_id
    );
    bool deleteHospital(int hospital_id);
    std::string selectAllUsers();
    std::string selectAllHospitals();

private:
    PGconn *connection_ = nullptr;
    const std::string connect_information_;

    static std::string prepare_Query(const std::string &query);
    static std::string generate_Salt(size_t length = 16);
    static std::string
    hash_Password(const std::string &password, const std::string &salt);
};

#endif  // DATABASE_HANDLER_HPP_
