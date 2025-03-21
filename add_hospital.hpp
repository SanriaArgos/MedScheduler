#ifndef ADD_HOSPITAL_HPP_
#define ADD_HOSPITAL_HPP_

#include "database.hpp"
#include <string>

// Функция для добавления новой больницы. Данные передаются в виде параметров.
bool add_hospital(const std::string &region,
                  const std::string &settlement_type,
                  const std::string &settlement_name,
                  const std::string &street,
                  const std::string &house,
                  const std::string &full_name,
                  int admin_id);

#endif  // ADD_HOSPITAL_HPP_
