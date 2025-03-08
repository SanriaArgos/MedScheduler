#include "database.hpp"
#include <ctime>
#include <iomanip>
#include <openssl/sha.h>
#include <random>
#include <sstream>
#include <stdexcept>

/**
 * @brief Конструктор, сохраняющий строку подключения.
 */
DatabaseHandler::DatabaseHandler(const std::string &conninfo)
    : connection_(nullptr), conninfo_(conninfo) {}

/**
 * @brief Деструктор, закрывающий соединение с базой данных, если оно установлено.
 */
DatabaseHandler::~DatabaseHandler() {
  if (connection_)
    PQfinish(connection_);
}

/**
 * @brief Устанавливает соединение с PostgreSQL.
 * @return true, если соединение успешно установлено.
 */
bool DatabaseHandler::connect() {
  connection_ = PQconnectdb(conninfo_.c_str());
  return PQstatus(connection_) == CONNECTION_OK;
}

/**
 * @brief Подготавливает строку для SQL-запроса, экранируя одинарные кавычки.
 */
std::string DatabaseHandler::prepareQuery(const std::string &query) {
  std::string cleaned = query;
  size_t pos;
  while ((pos = cleaned.find("'")) != std::string::npos) {
    cleaned.replace(pos, 1, "''");
  }
  return cleaned;
}

/**
 * @brief Генерирует случайную строку-соль заданной длины.
 */
std::string DatabaseHandler::generateSalt(size_t length) {
  static const char charset[] = "0123456789"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz";

  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

  std::string salt;
  salt.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    salt.push_back(charset[dist(engine)]);
  }
  return salt;
}

/**
 * @brief Хеширует пароль с солью с использованием SHA256.
 */
std::string DatabaseHandler::hashPassword(const std::string &password,
                                          const std::string &salt) {
  // Объединяем пароль и соль для хеширования
  std::string to_hash = password + salt;

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(reinterpret_cast<const unsigned char *>(to_hash.c_str()),
         to_hash.size(), hash);

  std::stringstream ss;
  // Преобразуем байты хеша в шестнадцатеричную строку
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return ss.str();
}

/**
 * @brief Проверяет, существует ли пользователь с указанным номером телефона.
 */
bool DatabaseHandler::userExists(const std::string &phone) {
  std::string query =
      "SELECT 1 FROM users WHERE phone = '" + prepareQuery(phone) + "'";
  PGresult *res = PQexec(connection_, query.c_str());
  bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
  PQclear(res);
  return exists;
}

/**
 * @brief Регистрирует нового пользователя: генерирует соль, хеширует пароль и вставляет данные в таблицу.
 */
bool DatabaseHandler::registerUser(const std::string &last_name,
                                   const std::string &first_name,
                                   const std::string &patronymic,
                                   const std::string &phone,
                                   const std::string &password) {
  // Генерация соли и вычисление хеша пароля
  std::string salt = generateSalt(16);
  std::string hashed_pass = hashPassword(password, salt);

  // Формирование SQL-запроса для вставки нового пользователя
  std::string query =
      "INSERT INTO users (last_name, first_name, patronymic, phone, "
      "hashed_password, salt, user_type) "
      "VALUES ('" +
      prepareQuery(last_name) + "', '" + prepareQuery(first_name) + "', '" +
      prepareQuery(patronymic) + "', '" + prepareQuery(phone) + "', '" +
      prepareQuery(hashed_pass) + "', '" + prepareQuery(salt) + "', 'patient')";

  PGresult *res = PQexec(connection_, query.c_str());
  bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
  PQclear(res);
  return success;
}

/**
 * @brief Аутентифицирует пользователя. Сначала ищет пользователя по телефону,
 * затем сравнивает хеш введённого пароля с хешем из базы.
 */
std::string DatabaseHandler::loginUser(const std::string &phone,
                                       const std::string &password) {
  std::string query = "SELECT id, user_type, hashed_password, salt "
                      "FROM users "
                      "WHERE phone = '" +
                      prepareQuery(phone) + "'";

  PGresult *res = PQexec(connection_, query.c_str());
  if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
    if (res)
      PQclear(res);
    return "";
  }

  // Извлекаем id, тип пользователя, хеш и соль из результата запроса
  std::string user_id = PQgetvalue(res, 0, 0);
  std::string user_type = PQgetvalue(res, 0, 1);
  std::string db_hashed_pass = PQgetvalue(res, 0, 2);
  std::string db_salt = PQgetvalue(res, 0, 3);
  PQclear(res);

  // Вычисляем хеш введённого пользователем пароля
  std::string attempt_hash = hashPassword(password, db_salt);

  // Сравниваем вычисленный хеш с хранящимся в базе
  if (attempt_hash != db_hashed_pass) {
    return "";
  }

  // Если пользователь – пациент, возвращаем "patient:<id>", иначе "success"
  if (user_type == "patient") {
    return "patient:" + user_id;
  }
  return "success";
}

/**
 * @brief Получает записи пациента, объединяя данные из таблиц records и hospitals.
 */
std::string DatabaseHandler::getPatientRecords(int patient_id) {
  std::string query = "SELECT r.time_and_date, h.address "
                      "FROM records r "
                      "JOIN hospitals h ON r.hospital_id = h.hospital_id "
                      "WHERE r.patient_id = " +
                      std::to_string(patient_id);

  PGresult *res = PQexec(connection_, query.c_str());
  if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
    if (res)
      PQclear(res);
    return "";
  }

  std::string result;
  int rows = PQntuples(res);

  // Формирование строки с данными каждой записи
  for (int i = 0; i < rows; ++i) {
    result += std::string(PQgetvalue(res, i, 0)) + " | " +
              PQgetvalue(res, i, 1) + "\n";
  }

  PQclear(res);
  return result;
}

/**
 * @brief Обновляет личную информацию пользователя.
 * 
 * Перед выполнением обновления проверяет, не занят ли новый номер телефона другим пользователем.
 */
bool DatabaseHandler::updateUserInfo(int user_id, const std::string &last_name,
                                     const std::string &first_name,
                                     const std::string &patronymic,
                                     const std::string &phone) {
  // Проверка наличия другого пользователя с таким же номером телефона
  std::string query_check =
      "SELECT id FROM users WHERE phone = '" + prepareQuery(phone) + "'";
  PGresult *res_check = PQexec(connection_, query_check.c_str());
  if (PQresultStatus(res_check) == PGRES_TUPLES_OK &&
      PQntuples(res_check) > 0) {
    std::string existing_id = PQgetvalue(res_check, 0, 0);
    // Если найденный id отличается от текущего, значит телефон уже используется
    if (std::to_string(user_id) != existing_id) {
      PQclear(res_check);
      return false; // Обновление не выполняется, телефон занят
    }
  }
  PQclear(res_check);

  // Формирование SQL-запроса для обновления информации пользователя
  std::string query =
      "UPDATE users SET last_name = '" + prepareQuery(last_name) +
      "', first_name = '" + prepareQuery(first_name) + "', patronymic = '" +
      prepareQuery(patronymic) + "', phone = '" + prepareQuery(phone) +
      "' WHERE id = " + std::to_string(user_id);

  PGresult *res = PQexec(connection_, query.c_str());
  bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
  PQclear(res);
  return success;
}
