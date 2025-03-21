#include "system_init.hpp"
#include "database.hpp"
#include "registration.hpp"
#include "login.hpp"
#include "add_doctor.hpp"
#include "add_hospital.hpp"
#include "add_junior_admin.hpp"
#include "add_record_slot.hpp"
#include "junior_admin_schedule.hpp"
#include "display_users.hpp"
#include "display_hospitals.hpp"
#include "display_doctors.hpp"
#include "doctor_menu.hpp"
#include "doctor_schedule.hpp"
#include "patient_schedule.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Глобальный указатель на объект базы данных, используемый во всех модулях.
database_handler* global_db = nullptr;

int main() {
    // Инициализация системы: создание PostgreSQL пользователя Meduser и базы данных Medscheduler (если отсутствуют).
    const std::string superuser_connect_info = "dbname=postgres user=postgres password=123 host=localhost port=5432";
    if (!initialize_system(superuser_connect_info)) {
        std::cerr << "System initialization error\n";
        return 1;
    }

    // Подключение к базе данных Medscheduler от имени Meduser.
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

    // Чтение входного JSON со стандартного ввода.
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

        // Для каждой операции входные данные передаются через JSON (в поле "data")
        if (action == "register") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for register action\n";
                return 1;
            }
            bool success = registration(j["data"]);
            response["action"] = "register";
            response["success"] = success;
        }
        else if (action == "login") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for login action\n";
                return 1;
            }
            response = login(j["data"]);
            response["action"] = "login";
        }
        else if (action == "add_doctor") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for add_doctor action\n";
                return 1;
            }
            bool success = add_doctor(j["data"]);
            response["action"] = "add_doctor";
            response["success"] = success;
        }
        else if (action == "add_hospital") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for add_hospital action\n";
                return 1;
            }
            bool success = add_hospital(j["data"]);
            response["action"] = "add_hospital";
            response["success"] = success;
        }
        else if (action == "add_junior_admin") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for add_junior_admin action\n";
                return 1;
            }
            bool success = add_junior_admin(j["data"]);
            response["action"] = "add_junior_admin";
            response["success"] = success;
        }
        else if (action == "add_record_slot") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for add_record_slot action\n";
                return 1;
            }
            bool success = add_record_slot(j["data"]);
            response["action"] = "add_record_slot";
            response["success"] = success;
        }
        else if (action == "junior_admin_schedule") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for junior_admin_schedule action\n";
                return 1;
            }
            response = junior_admin_schedule(j["data"]);
            response["action"] = "junior_admin_schedule";
        }
        else if (action == "display_users") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for display_users action\n";
                return 1;
            }
            response = display_users_table(j["data"]);
            response["action"] = "display_users";
        }
        else if (action == "display_hospitals") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for display_hospitals action\n";
                return 1;
            }
            response = display_hospitals_table(j["data"]);
            response["action"] = "display_hospitals";
        }
        else if (action == "display_doctors") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for display_doctors action\n";
                return 1;
            }
            response = display_doctors_table(j["data"]);
            response["action"] = "display_doctors";
        }
        else if (action == "doctor_schedule") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for doctor_schedule action\n";
                return 1;
            }
            response = display_doctor_schedule(j["data"]);
            response["action"] = "doctor_schedule";
        }
        else if (action == "patient_schedule") {
            if (!j.contains("data")) {
                std::cerr << "Missing 'data' field for patient_schedule action\n";
                return 1;
            }
            response = view_doctor_schedule_for_patient(j["data"]);
            response["action"] = "patient_schedule";
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
