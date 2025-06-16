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
    json get_regions();           // region
    json get_settlement_types();  // settlement_type
    json get_settlement_names();  // settlement_name

    // Просмотр расписания у выбранного врача в выбранной клинике
    json get_doctor_schedule_for_patient(int doctor_id);

    // Оставить отзыв (возвращает JSON успех/ошибка)
    json post_doctor_feedback_client(const json &request_data);
    json get_hospital_full_names();
    json get_specialties();

    json book_appointment(const json &request_data);
    json search_doctors(const json &request_data);

    json get_patient_appointments(int patient_id);

    json get_patient_appointments(const json &request_data);

    json get_doctor_average_ratings();
    json get_doctor_feedback_items(int doctor_id);
    json get_doctor_feedback_calculated(int doctor_id);
    json get_doctor_hospitals(int doctor_id);

    json add_patient_to_waitlist(const json &request_data);

    json edit_doctor_feedback(const json &request_data);

    json cancel_appointment(const json &data);

    json cancel_waitlist(const json &data);
    json delete_self_account(int patient_id);

    json get_cancelled_slots(int doctor_id);

    json get_waitlist_count(int doctor_id);

    json edit_patient_profile(const json &request_data);

    json get_patient_profile_by_id(int user_id);

private:
    int user_id_;
};

}  // namespace patient

#endif  // CLIENT_PATIENT_HPP_
