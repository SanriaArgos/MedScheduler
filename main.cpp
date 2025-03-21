#include "system_init.hpp"
#include "database.hpp"
#include "registration.hpp"
#include "login.hpp"
#include "add_doctor.hpp"
#include "add_hospital.hpp"
#include "add_junior_admin.hpp"
#include "add_record_slot.hpp"
#include "doctor_schedule.hpp"
#include "junior_admin_schedule.hpp"
#include "patient_schedule.hpp"
#include "display_doctors.hpp"
#include "display_hospitals.hpp"
#include "display_users.hpp"
#include "doctor_hospital_management.hpp"
#include "doctor_menu.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Глобальный указатель для использования в модулях, где нельзя обращаться к клиентской логике.
database_handler* global_db = nullptr;

int main() {
    const std::string superuser_connect_info = "dbname=postgres user=postgres password=123 host=localhost port=5432";
    if (!initialize_system(superuser_connect_info)) {
        std::cerr << "System initialization error\n";
        return 1;
    }

    const std::string connect_information = "dbname=medscheduler user=meduser password=3671920119 host=localhost port=5432";
    database_handler db(connect_information);
    if (!db.connect()) {
        std::cerr << "Database connection error\n";
        return 1;
    }
    if (!db.initialize_database()) {
        std::cerr << "Database initialization error\n";
        return 1;
    }
    global_db = &db;

    std::string input_json;
    std::string line;
    while (std::getline(std::cin, line)) {
        input_json += line;
    }
    if (input_json.empty()) {
        std::cerr << "No input provided\n";
        return 1;
    }

    try {
        json j = json::parse(input_json);
        if (!j.contains("action")) {
            std::cerr << "JSON input missing 'action' field\n";
            return 1;
        }
        std::string action = j["action"];
        json response;
        
        if (action == "register") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for register action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("last_name") || !data.contains("first_name") ||
                !data.contains("phone") || !data.contains("password")) {
                std::cerr << "Missing required registration fields\n";
                return 1;
            }
            std::string last_name = data["last_name"];
            std::string first_name = data["first_name"];
            std::string patronymic = data.value("patronymic", "");
            std::string phone = data["phone"];
            std::string password = data["password"];
            bool success = db.register_user(last_name, first_name, patronymic, phone, password);
            response["action"] = "register";
            response["success"] = success;
        }
        else if (action == "login") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for login action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("phone") || !data.contains("password")) {
                std::cerr << "Missing required login fields\n";
                return 1;
            }
            std::string phone = data["phone"];
            std::string password = data["password"];
            std::string login_result = db.login_user(phone, password);
            bool success = !login_result.empty();
            response["action"] = "login";
            response["success"] = success;
            if (success) {
                response["result"] = login_result;
            }
        }
        else if (action == "add_doctor") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for add_doctor action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("last_name") || !data.contains("first_name") ||
                !data.contains("phone") || !data.contains("education") ||
                !data.contains("specialty") || !data.contains("experience")) {
                std::cerr << "Missing required add_doctor fields\n";
                return 1;
            }
            std::string last_name = data["last_name"];
            std::string first_name = data["first_name"];
            std::string patronymic = data.value("patronymic", "");
            std::string phone = data["phone"];
            std::string education = data["education"];
            std::string specialty = data["specialty"];
            int experience = data["experience"];
            bool success = add_doctor(last_name, first_name, patronymic, phone, education, specialty, experience);
            response["action"] = "add_doctor";
            response["success"] = success;
        }
        else if (action == "add_hospital") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for add_hospital action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("region") || !data.contains("settlement_type") ||
                !data.contains("settlement_name") || !data.contains("street") ||
                !data.contains("house") || !data.contains("full_name") ||
                !data.contains("admin_id")) {
                std::cerr << "Missing required add_hospital fields\n";
                return 1;
            }
            std::string region = data["region"];
            std::string settlement_type = data["settlement_type"];
            std::string settlement_name = data["settlement_name"];
            std::string street = data["street"];
            std::string house = data["house"];
            std::string full_name = data["full_name"];
            int admin_id = data["admin_id"];
            bool success = add_hospital(region, settlement_type, settlement_name, street, house, full_name, admin_id);
            response["action"] = "add_hospital";
            response["success"] = success;
        }
        else if (action == "add_junior_admin") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for add_junior_admin action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("last_name") || !data.contains("first_name") || !data.contains("phone")) {
                std::cerr << "Missing required add_junior_admin fields\n";
                return 1;
            }
            std::string last_name = data["last_name"];
            std::string first_name = data["first_name"];
            std::string patronymic = data.value("patronymic", "");
            std::string phone = data["phone"];
            bool success = add_junior_admin(last_name, first_name, patronymic, phone);
            response["action"] = "add_junior_admin";
            response["success"] = success;
        }
        else if (action == "add_record_slot") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for add_record_slot action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("doctor_id") || !data.contains("date") ||
                !data.contains("time") || !data.contains("hospital_id") ||
                !data.contains("cabinet") || !data.contains("junior_admin_id")) {
                std::cerr << "Missing required add_record_slot fields\n";
                return 1;
            }
            int doctor_id = data["doctor_id"];
            std::string date = data["date"];
            std::string time_val = data["time"];
            int hospital_id = data["hospital_id"];
            int cabinet = data["cabinet"];
            int junior_admin_id = data["junior_admin_id"];
            bool success = add_record_slot(doctor_id, date, time_val, hospital_id, cabinet, junior_admin_id);
            response["action"] = "add_record_slot";
            response["success"] = success;
        }
        else if (action == "display_doctor_schedule") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for display_doctor_schedule action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("doctor_id")) {
                std::cerr << "Missing required display_doctor_schedule field: doctor_id\n";
                return 1;
            }
            int doctor_id = data["doctor_id"];
            bool success = display_doctor_schedule(db, doctor_id);
            response["action"] = "display_doctor_schedule";
            response["success"] = success;
        }
        else if (action == "junior_admin_schedule") {
            if (!j.contains("data")) {
                std::cerr << "JSON input missing 'data' field for junior_admin_schedule action\n";
                return 1;
            }
            auto data = j["data"];
            if (!data.contains("junior_admin_id")) {
                std::cerr << "Missing required junior_admin_schedule field: junior_admin_id\n";
                return 1;
            }
            int junior_admin_id = data["junior_admin_id"];
            bool success = junior_admin_schedule(db, junior_admin_id);
            response["action"] = "junior_admin_schedule";
            response["success"] = success;
        }
        else if (action == "view_doctor_schedule_for_patient") {
            bool success = view_doctor_schedule_for_patient(db);
            response["action"] = "view_doctor_schedule_for_patient";
            response["success"] = success;
        }
        else {
            std::cerr << "Unknown action: " << action << "\n";
            return 1;
        }
        
        std::cout << response.dump(4) << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
