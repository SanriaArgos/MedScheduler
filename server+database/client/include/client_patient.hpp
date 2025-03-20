#ifndef CLIENT_PATIENT_HPP
#define CLIENT_PATIENT_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace patient {

using json = nlohmann::json;

class patient_client {
public:
    void view_doctor_schedule();
    void run_menu(); // Метод для запуска меню
};

} // namespace patient

#endif // CLIENT_PATIENT_HPP