#ifndef SUPER_ADMIN_MENU_HPP_
#define SUPER_ADMIN_MENU_HPP_

#include "handlers.hpp"

/**
 * @brief Меню личного кабинета главного администратора.
 * @param db Ссылка на объект базы данных.
 * @param admin_id Идентификатор главного администратора.
 */
void superAdminMenu(Database_Handler &db, int admin_id);

#endif  // SUPER_ADMIN_MENU_HPP_
