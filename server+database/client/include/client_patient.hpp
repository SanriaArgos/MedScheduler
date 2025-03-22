#ifndef CLIENT_PATIENT_HPP
#define CLIENT_PATIENT_HPP

#include <string>

namespace patient {

class patient_client {
public:
    patient_client(int user_id);
    void run_menu();

private:
    int user_id; 
    void view_doctor_schedule();
};

} // namespace patient

#endif // CLIENT_PATIENT_HPP