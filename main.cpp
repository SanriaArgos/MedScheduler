#include "auth.hpp"
#include "database.hpp"
#include <iostream>

/**
 * @brief Отображает записи пациента, полученные из базы данных.
 * @param db Объект для работы с базой данных.
 * @param patient_id Идентификатор пациента.
 */
void showPatientCabinet(DatabaseHandler &db, int patient_id) {
  std::cout << "\n=== Личный кабинет ===" << std::endl;

  // Получаем записи пациента из БД
  std::string records = db.getPatientRecords(patient_id);
  if (records.empty()) {
    std::cout << "У вас пока нет записей" << std::endl;
  } else {
    std::cout << "Ваши записи:\n" << records << std::endl;
  }
}

/**
 * @brief Позволяет пользователю отредактировать свою личную информацию.
 * 
 * Запрашивает новые значения для фамилии, имени, отчества и номера телефона,
 * проверяет корректность ввода и вызывает обновление в базе данных.
 *
 * @param db Объект для работы с базой данных.
 * @param patient_id Идентификатор пользователя.
 */
void editProfile(DatabaseHandler &db, int patient_id) {
  std::string new_last_name, new_first_name, new_patronymic, new_phone;

  std::cout << "\n=== Редактирование личной информации ===" << std::endl;

  // Запрос нового значения фамилии
  std::cout << "Введите новую фамилию: ";
  std::getline(std::cin, new_last_name);
  new_last_name = Auth::trimWhitespace(new_last_name);

  // Запрос нового значения имени
  std::cout << "Введите новое имя: ";
  std::getline(std::cin, new_first_name);
  new_first_name = Auth::trimWhitespace(new_first_name);

  // Запрос нового значения отчества
  std::cout << "Введите новое отчество: ";
  std::getline(std::cin, new_patronymic);
  new_patronymic = Auth::trimWhitespace(new_patronymic);

  // Запрос нового номера телефона с проверкой формата
  while (true) {
    std::cout << "Введите новый телефон (с +): ";
    std::getline(std::cin, new_phone);
    new_phone = Auth::trimWhitespace(new_phone);
    if (!Auth::validatePhone(new_phone)) {
      std::cout << "Ошибка: неверный формат телефона!\n";
      continue;
    }
    break;
  }

  // Если хотя бы одно поле содержит пробелы, выводим ошибку
  if (new_last_name.find(' ') != std::string::npos ||
      new_first_name.find(' ') != std::string::npos ||
      new_patronymic.find(' ') != std::string::npos ||
      new_phone.find(' ') != std::string::npos) {
    std::cout << "Ошибка: пробелы недопустимы во входных данных.\n";
    return;
  }

  // Пытаемся обновить информацию в базе данных
  if (db.updateUserInfo(patient_id, new_last_name, new_first_name,
                        new_patronymic, new_phone)) {
    std::cout << "Личная информация успешно обновлена.\n";
  } else {
    std::cout << "Ошибка при обновлении информации.\n";
  }
}

/**
 * @brief Основная функция, реализующая меню регистрации, авторизации и работы личного кабинета.
 */
int main() {
  // Строка подключения к базе данных
  const std::string conninfo = "dbname=medscheduler user=meduser "
                               "password=3671920119 host=localhost port=5432";
  DatabaseHandler db(conninfo);

  // Пытаемся установить соединение с БД
  if (!db.connect()) {
    std::cerr << "Ошибка подключения к БД" << std::endl;
    return 1;
  }

  // Главное меню приложения
  while (true) {
    std::cout << "\n1. Регистрация\n2. Вход\n3. Выход\nВыберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Очистка буфера ввода

    if (choice == 3)
      break;

    if (choice == 1) { // Регистрация нового пользователя
      std::string last_name, first_name, patronymic, phone, pass1, pass2;
      std::cout << "Внимание: пробелы недопустимы во входных данных!\n";

      // Ввод фамилии
      while (true) {
        std::cout << "Фамилия: ";
        std::getline(std::cin, last_name);
        last_name = Auth::trimWhitespace(last_name);
        if (last_name.empty()) {
          std::cout << "Ошибка: поле обязательно для заполнения!\n";
          continue;
        }
        if (last_name.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        break;
      }

      // Ввод имени
      while (true) {
        std::cout << "Имя: ";
        std::getline(std::cin, first_name);
        first_name = Auth::trimWhitespace(first_name);
        if (first_name.empty()) {
          std::cout << "Ошибка: поле обязательно для заполнения!\n";
          continue;
        }
        if (first_name.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        break;
      }

      // Ввод отчества (необязательное поле)
      std::cout << "Отчество (необязательно): ";
      std::getline(std::cin, patronymic);
      patronymic = Auth::trimWhitespace(patronymic);
      if (!patronymic.empty() && patronymic.find(' ') != std::string::npos) {
        std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
        // Предлагаем повторный ввод, если отчество содержит пробелы
        while (patronymic.find(' ') != std::string::npos) {
          std::cout << "Отчество (необязательно, без пробелов): ";
          std::getline(std::cin, patronymic);
          patronymic = Auth::trimWhitespace(patronymic);
        }
      }

      // Ввод номера телефона
      while (true) {
        std::cout << "Телефон (с +): ";
        std::getline(std::cin, phone);
        phone = Auth::trimWhitespace(phone);
        if (!Auth::validatePhone(phone)) {
          std::cout << "Ошибка: неверный формат телефона!\n";
          continue;
        }
        if (phone.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        if (db.userExists(phone)) {
          std::cout << "Ошибка: телефон уже зарегистрирован!\n";
          continue;
        }
        break;
      }

      // Ввод пароля и его подтверждения
      while (true) {
        std::cout << "Пароль: ";
        std::getline(std::cin, pass1);
        pass1 = Auth::trimWhitespace(pass1);
        if (pass1.empty()) {
          std::cout << "Ошибка: пароль не может быть пустым!\n";
          continue;
        }
        if (pass1.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        std::cout << "Повторите пароль: ";
        std::getline(std::cin, pass2);
        pass2 = Auth::trimWhitespace(pass2);
        if (pass2.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        if (pass1 != pass2) {
          std::cout << "Ошибка: пароли не совпадают!\n";
          continue;
        }
        break;
      }

      // Регистрация пользователя
      if (db.registerUser(last_name, first_name, patronymic, phone, pass1)) {
        std::cout << "Регистрация прошла успешно!\n";
      } else {
        std::cout << "Ошибка регистрации!\n";
      }
    } else if (choice == 2) { // Авторизация пользователя
      std::string phone, password;

      // Ввод номера телефона с проверкой
      while (true) {
        std::cout << "Телефон (с +): ";
        std::getline(std::cin, phone);
        phone = Auth::trimWhitespace(phone);

        if (!Auth::validatePhone(phone)) {
          std::cout << "Ошибка: неверный формат телефона!\n";
          continue;
        }
        if (phone.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        break;
      }

      // Ввод пароля с проверкой
      while (true) {
        std::cout << "Пароль: ";
        std::getline(std::cin, password);
        password = Auth::trimWhitespace(password);

        if (password.find(' ') != std::string::npos) {
          std::cout << "Ошибка: пробелы недопустимы во входных данных!\n";
          continue;
        }
        break;
      }

      // Проводим аутентификацию пользователя
      std::string login_result = db.loginUser(phone, password);
      if (login_result.empty()) {
        std::cout << "Неверные данные!\n";
      } else if (login_result.rfind("patient:", 0) == 0) {
        // Если аутентификация успешна и пользователь - пациент,
        // извлекаем идентификатор и показываем меню личного кабинета
        int patient_id = std::stoi(login_result.substr(8));
        int patient_choice = 0;
        do {
          std::cout << "\n=== Личный кабинет ===\n";
          std::cout << "1. Просмотр записей\n2. Редактировать личную информацию\n3. Выход\nВыберите действие: ";
          std::cin >> patient_choice;
          std::cin.ignore(); // Очистка буфера ввода
          if (patient_choice == 1) {
            showPatientCabinet(db, patient_id);
          } else if (patient_choice == 2) {
            editProfile(db, patient_id);
          }
        } while (patient_choice != 3);
      } else {
        std::cout << "Вход выполнен!\n";
      }
    }
  }

  return 0;
}
