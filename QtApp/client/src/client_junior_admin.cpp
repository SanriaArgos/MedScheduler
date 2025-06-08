#include "client_junior_admin.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace junior_admin {

using json = nlohmann::json;

junior_admin_client::junior_admin_client(int admin_id) : admin_id(admin_id) {
}

json junior_admin_client::get_doctor_schedule(int doctor_id) {
    std::string check_url =
        base_url +
        "/check_doctor_admin_hospital?doctor_id=" + std::to_string(doctor_id) +
        "&admin_id=" + std::to_string(admin_id);
    std::string check_response = send_get_request(check_url);
    try {
        std::cerr << "Raw response: " << check_response << std::endl;
        json check_result = json::parse(check_response);
        if (!check_result.value("is_valid", false)) {
            std::cerr << "Error: Doctor and admin are not associated with the "
                         "same hospital.\n";
            return json();
        }
        // вывести в интерфейсе что-то по типу "вы не можете посмотреть
        // расписание у этого врача, так как он не прикреплен к вашей больнице"
    } catch (const std::exception &e) {
        std::cerr << "Error checking hospital association: " << e.what()
                  << std::endl;
        return json();
    }

    std::string schedule_url = base_url + "/get_doctor_schedule?doctor_id=" +
                               std::to_string(doctor_id);
    std::string schedule_response = send_get_request(schedule_url);

    try {
        std::cerr << "Raw response: " << schedule_response << std::endl;
        json schedule = json::parse(schedule_response);
        return schedule;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching doctor's schedule: " << e.what()
                  << std::endl;
        return json();
    }
}

void junior_admin_client::add_doctor(const json &data) {
    std::string url = base_url + "/add_doctor";
    std::string response = send_post_request(url, data);
}

void junior_admin_client::attach_doctor_to_hospital_class(const json &data) {
    int doctor_id = data.value("doctor_id", -1);
    int hospital_id = data.value("hospital_id", -1);

    std::cout << doctor_id << " " << hospital_id << "\n";

    if (doctor_id == -1 || hospital_id == -1) {
        std::cerr << "Invalid JSON: missing or wrong type for doctor_id / "
                     "hospital_id\n";
        return;
    }

    json payload = {
        {"doctor_id", doctor_id},
        {"hospital_id", hospital_id},
        {"junior_admin_id", admin_id}};

    std::string url = base_url + "/attach_doctor_to_hospital";
    std::string response = send_post_request(url, payload);
    std::cerr << "attach response: " << response << std::endl;
}

void junior_admin_client::add_record_slot(const json &data) {
    std::string url = base_url + "/add_record_slot";
    std::string response = send_post_request(url, data);
}

bool junior_admin_client::check_doctor_exists(int doctor_id) {
    std::string url = base_url + "/doctors_exist/" + std::to_string(doctor_id);
    std::string response = send_get_request(url);

    if (response.empty()) {
        std::cerr << "Error: empty response from server" << std::endl;
        return false;
    }

    try {
        json doctor_data = json::parse(response);

        if (doctor_data.contains("exists") &&
            doctor_data["exists"].is_boolean()) {
            return doctor_data["exists"];
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return false;
}

bool junior_admin_client::check_hospital_exists(int hospital_id) {
    std::string url =
        base_url + "/hospitals_exist/" + std::to_string(hospital_id);
    std::string response = send_get_request(url);

    if (response.empty()) {
        std::cerr << "Error: empty response from server" << std::endl;
        return false;
    }

    try {
        json hospital_data = json::parse(response);

        if (hospital_data.contains("exists") &&
            hospital_data["exists"].is_boolean()) {
            return hospital_data["exists"];
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return false;
}

bool junior_admin_client::is_doctor_attached_to_hospital(
    int doctor_id,
    int hospital_id
) {
    std::string url =
        base_url + "/doctors/" + std::to_string(doctor_id) + "/hospitals";
    std::string response = send_get_request(url);

    try {
        json hospital_list = json::parse(response);
        for (const auto &hospital : hospital_list) {
            if (hospital["hospital_id"] == hospital_id) {
                return true;  // Врач уже привязан к этой больнице
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error checking doctor's hospitals: " << e.what()
                  << std::endl;
    }
    return false;  // Врач не привязан к этой больнице
}

void junior_admin_client::detach_doctor_from_hospital(const json &data) {
    int doctor_id = data.value("doctor_id", -1);
    int hospital_id = data.value("hospital_id", -1);

    if (doctor_id < 0 || hospital_id < 0) {
        std::cerr << "Invalid JSON: missing or wrong type for doctor_id / "
                     "hospital_id\n";
        return;
    }
    if (!check_doctor_exists(doctor_id)) {
        std::cerr << "Error: Doctor with ID " << doctor_id << " not found.\n";
        return;
    }

    if (!check_hospital_exists(hospital_id)) {
        std::cerr << "Error: Hospital with ID " << hospital_id
                  << " not found.\n";
        return;
    }

    json payload = {
        {"doctor_id", doctor_id},
        {"hospital_id", hospital_id},
        {"junior_admin_id", admin_id}  // <— make sure the server sees it!
    };

    std::string url = base_url + "/detach_doctor_from_hospital";
    std::string response = send_post_request(url, payload);
    std::cerr << "detach response: " << response << std::endl;
}

json junior_admin_client::get_doctors_table() {
    std::string url = base_url + "/get_doctors";
    std::string response = send_get_request(url);

    try {
        json doctors = json::parse(response);
        return doctors;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching doctors table: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_users_table() {
    std::string url = base_url + "/get_users";
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching users table: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::delete_doctor_feedback(const json &data) {
    std::string url = base_url + "/delete_doctor_feedback";
    std::string response = send_delete_request(url, data);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error with feedback: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::edit_junior_admin_profile(const json &data) {
    std::string url = base_url + "/edit_junior_admin_profile";
    std::string response = send_patch_request(url, data);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error with feedback: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_waitlist(int doctor_id, int junior_admin_id) {
    std::string url = base_url +
                      "/get_waitlist?doctor_id=" + std::to_string(doctor_id) +
                      "&junior_admin_id" + std::to_string(junior_admin_id);
    std::string response = send_get_request(url);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error with feedback: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_doctor_statistics(const json &data) {
    std::string url = base_url + "/get_doctor_statistics";
    std::string response = send_post_request(url, data);

    try {
        json users = json::parse(response);
        return users;
    } catch (const std::exception &e) {
        std::cerr << "Error with feedback: " << e.what() << std::endl;
        return json();
    }
}

json junior_admin_client::get_junior_admin_profile_by_id(int user_id) {
    std::string url = base_url + "/get_profile_by_id" + std::to_string(user_id);
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

}  // namespace junior_admin
