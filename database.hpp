#ifndef DATABASE_HANDLER_HPP_
#define DATABASE_HANDLER_HPP_

#include <libpq-fe.h>
#include <string>

/**
 * @brief Класс Database_Handler отвечает за подключение к базе данных PostgreSQL и выполнение SQL-запросов.
 */
class Database_Handler {
public:
    /**
     * @brief Конструктор класса, принимает строку подключения.
     * Объявлен explicit для предотвращения неявных преобразований.
     * @param connect_information Строка подключения к БД.
     */
    explicit Database_Handler(const std::string &connect_information);

    /**
     * @brief Деструктор, закрывающий соединение с базой данных.
     */
    ~Database_Handler();

    /**
     * @brief Устанавливает соединение с базой данных.
     * @return true, если соединение успешно установлено.
     */
    bool connect();

    /**
     * @brief Проверяет, существует ли пользователь с указанным номером телефона среди пациентов.
     * @param phone Номер телефона.
     * @return true, если пользователь найден.
     */
    bool user_Exists(const std::string &phone);

    /**
     * @brief Регистрирует нового пользователя в базе.
     * @param last_name Фамилия.
     * @param first_name Имя.
     * @param patronymic Отчество (необязательно).
     * @param phone Номер телефона.
     * @param password Пароль (будет захеширован).
     * @return true, если регистрация прошла успешно.
     */
    bool register_User(const std::string &last_name, const std::string &first_name,
                      const std::string &patronymic, const std::string &phone,
                      const std::string &password);

    /**
     * @brief Проводит аутентификацию пользователя.
     * @param phone Номер телефона.
     * @param password Пароль.
     * @return Строка вида "patient:<id>" для пациентов, "success" для других,
     *         либо пустая строка при ошибке.
     */
    std::string login_User(const std::string &phone, const std::string &password);

    /**
     * @brief Получает записи пациента из таблицы records.
     * @param patient_id Идентификатор пациента.
     * @return Строка с информацией о записях.
     */
    std::string get_Patient_Records(int patient_id);

    /**
     * @brief Обновляет личную информацию пользователя.
     * @param user_id Идентификатор пользователя.
     * @param last_name Новая фамилия.
     * @param first_name Новое имя.
     * @param patronymic Новое отчество.
     * @param phone Новый номер телефона.
     * @return true, если обновление прошло успешно, false если, например, новый номер уже используется.
     */
    bool update_User_Info(int user_id, const std::string &last_name,
                        const std::string &first_name,
                        const std::string &patronymic, const std::string &phone);

private:
    PGconn *connection_ = nullptr;             ///< Соединение с базой данных
    const std::string connect_information_;    ///< Строка подключения

    /**
     * @brief Подготавливает строку для безопасного SQL-запроса, экранируя одинарные кавычки.
     * @param query Входная строка.
     * @return Экранированная строка.
     */
    static std::string prepare_Query(const std::string &query);

    /**
     * @brief Генерирует случайную строку-соль заданной длины.
     * @param length Длина соли (по умолчанию 16 символов).
     * @return Сгенерированная строка соли.
     */
    static std::string generate_Salt(size_t length = 16);

    /**
     * @brief Хеширует пароль, объединяя его с солью, с использованием SHA256.
     * @param password Пароль пользователя.
     * @param salt Соль.
     * @return Хеш в виде шестнадцатеричной строки.
     */
    static std::string hash_Password(const std::string &password, const std::string &salt);
};

#endif  // DATABASE_HANDLER_HPP_
