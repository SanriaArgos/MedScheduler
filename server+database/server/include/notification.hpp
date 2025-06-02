#pragma once

#include <string>

// Функция для отправки SMS с использованием захардкоженных учетных данных
// Принимает:
// - phoneNumber: Номер телефона получателя
// - textMessage: Текст сообщения
// Возвращает: Строку с ответом от сервера
std::string sendSms(const std::string& phoneNumber, const std::string& textMessage);

// Функция для отправки SMS с указанными учетными данными
// Принимает:
// - login: Ваш логин для API
// - password: Ваш пароль для API
// - phoneNumber: Номер телефона получателя
// - textMessage: Текст сообщения
// Возвращает: Строку с ответом от сервера
std::string sendSmsWithAuth(const std::string& login, const std::string& password,
                          const std::string& phoneNumber, const std::string& textMessage);
