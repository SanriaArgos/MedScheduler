#ifndef DATABASE_HANDLER_HPP_
#define DATABASE_HANDLER_HPP_

#include <libpq-fe.h>
#include <string>

/**
 * @brief Класс DatabaseHandler отвечает за подключение к базе данных PostgreSQL и выполнение SQL-запросов.
 */
class DatabaseHandler {
public:
  /**
   * @brief Конструктор класса, принимает строку подключения.
   * @param conninfo Строка подключения к БД.
   */
  DatabaseHandler(const std::string &conninfo);

  /**
   * @brief Деструктор, закрывающий соединение с базой данных.
   */
  ~DatabaseHandler();

  /**
   * @brief Устанавливает соединение с базой данных.
   * @return true, если соединение успешно установлено.
   */
  bool connect();

  /**
   * @brief Проверяет, существует ли пользователь с указанным номером телефона.
   * @param phone Номер телефона.
   * @return true, если пользователь найден.
   */
  bool userExists(const std::string &phone);

  /**
   * @brief Регистрирует нового пользователя в базе.
   * @param last_name Фамилия.
   * @param first_name Имя.
   * @param patronymic Отчество (может быть пустым).
   * @param phone Номер телефона.
   * @param password Пароль (входной пароль, будет захеширован).
   * @return true, если регистрация прошла успешно.
   */
  bool registerUser(const std::string &last_name, const std::string &first_name,
                    const std::string &patronymic, const std::string &phone,
                    const std::string &password);

  /**
   * @brief Проводит аутентификацию пользователя.
   * @param phone Номер телефона.
   * @param password Пароль.
   * @return Строка вида "patient:<id>" для пациентов или "success" для других типов пользователей,
   *         либо пустая строка при неудачной аутентификации.
   */
  std::string loginUser(const std::string &phone, const std::string &password);

  /**
   * @brief Получает записи пациента из таблицы records.
   * @param patient_id Идентификатор пациента.
   * @return Строка с информацией о записях.
   */
  std::string getPatientRecords(int patient_id);

  /**
   * @brief Обновляет личную информацию пользователя.
   * @param user_id Идентификатор пользователя.
   * @param last_name Новая фамилия.
   * @param first_name Новое имя.
   * @param patronymic Новое отчество.
   * @param phone Новый номер телефона.
   * @return true, если обновление прошло успешно, false если, например, новый номер уже используется.
   */
  bool updateUserInfo(int user_id, const std::string &last_name,
                      const std::string &first_name,
                      const std::string &patronymic, const std::string &phone);

private:
  PGconn *connection_;      ///< Соединение с базой данных
  std::string conninfo_;    ///< Строка подключения

  /**
   * @brief Подготавливает строку для безопасного SQL-запроса, экранируя одинарные кавычки.
   * @param query Входная строка.
   * @return Экранированная строка.
   */
  std::string prepareQuery(const std::string &query);

  /**
   * @brief Генерирует случайную строку-соль заданной длины.
   * @param length Длина соли (по умолчанию 16 символов).
   * @return Сгенерированная строка соли.
   */
  std::string generateSalt(size_t length = 16);

  /**
   * @brief Хеширует пароль, объединяя его с солью, с использованием алгоритма SHA256.
   * @param password Пароль пользователя.
   * @param salt Соль.
   * @return Хеш в виде шестнадцатеричной строки.
   */
  std::string hashPassword(const std::string &password,
                           const std::string &salt);
};

#endif // DATABASE_HANDLER_HPP_
