// client/include/client_patient.hpp

#ifndef CLIENT_PATIENT_HPP_
#define CLIENT_PATIENT_HPP_

#include <nlohmann/json.hpp>
#include <string>

namespace patient {

using json = nlohmann::json;

class patient_client {
public:
    explicit patient_client(int user_id);

    // Фильтры для поиска врачей
    json get_doctors_for_patient(const json &request_data);
    json get_regions();              // region
    json get_settlement_types();     // settlement_type
    json get_settlement_names();     // settlement_name

    // Просмотр расписания у выбранного врача в выбранной клинике
    json get_doctor_schedule_for_patient(int doctor_id, int hospital_id);

    // Оставить отзыв (возвращает JSON успех/ошибка)
    json post_doctor_feedback_client(const json &request_data);

    json book_appointment(const json &request_data);
    json search_doctors(const json &request_data);

    json patient_appointments(int patient_id);

    json patient_appointments(const json &request_data);

    // (устаревшая) просмотр расписания через POST
    // void view_doctor_schedule(const json &request_data);

private:
    int user_id_;
};

}  // namespace patient

#endif  // CLIENT_PATIENT_HPP_
