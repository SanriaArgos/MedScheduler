#ifndef SYSTEM_INIT_HPP_
#define SYSTEM_INIT_HPP_

#include <string>

// Функция инициализации системы: создание пользователя meduser и базы данных
// medscheduler, если их не существует.
bool initialize_system(const std::string &superuser_connect_info);

#endif  // SYSTEM_INIT_HPP_