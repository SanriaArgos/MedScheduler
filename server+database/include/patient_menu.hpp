#ifndef PATIENT_MENU_HPP_
#define PATIENT_MENU_HPP_

#include "handlers.hpp"

/**
 * @brief Функция для редактирования профиля пациента.
 * @param db Объект базы данных.
 * @param patient_id ID пациента.
 */
void edit_Profile(Database_Handler &db, int patient_id);

/**
 * @brief Меню личного кабинета пациента.
 * @param db Объект базы данных.
 * @param patient_id ID пациента.
 */
// void patient_Menu(Database_Handler &db, int patient_id);

/**
 * @brief Обрабатывает регистрацию пользователя.
 * @param db Объект базы данных.
 */
void handle_Registration(Database_Handler &db);

/**
 * @brief Обрабатывает вход пользователя.
 * @param db Объект базы данных.
 * @return Результат входа.
 */
std::string handle_Login(Database_Handler &db);

#endif // PATIENT_MENU_HPP_
