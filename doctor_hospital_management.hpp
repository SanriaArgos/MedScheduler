#ifndef DOCTOR_HOSPITAL_MANAGEMENT_HPP_
#define DOCTOR_HOSPITAL_MANAGEMENT_HPP_

// Добавляет идентификатор больницы (принадлежащей младшему администратору) в список врача.
// Параметры:
//   doctor_id  – идентификатор врача,
//   hospital_id – идентификатор больницы, которую требуется добавить,
//   junior_admin_id – идентификатор младшего администратора, ответственного за больницу.
// Возвращает true при успешном выполнении, false – при ошибке.
bool add_hospital_to_doctor(int doctor_id, int hospital_id, int junior_admin_id);

// Удаляет идентификатор больницы (принадлежащей младшему администратору) из списка врача.
// Параметры аналогичны функции добавления.
// Возвращает true при успешном выполнении, false – при ошибке.
bool remove_hospital_from_doctor(int doctor_id, int hospital_id, int junior_admin_id);

#endif  // DOCTOR_HOSPITAL_MANAGEMENT_HPP_
