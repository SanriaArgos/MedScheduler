#include "patient_schedule.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <tuple>

// Structure to hold search criteria.
struct search_criteria {
    std::string region;
    std::string settlement_type;
    std::string settlement_name;
    std::string specialty;
};

// Retrieves unique string values from the database for a given query.
std::vector<std::string> get_unique_values(database_handler &db, const std::string &query) {
    std::vector<std::string> values;
    PGresult *res = PQexec(db.get_connection(), query.c_str());
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            values.push_back(PQgetvalue(res, i, 0));
        }
    }
    PQclear(res);
    return values;
}

// Presents a numbered list of options and returns the selected option.
std::string select_from_list(const std::string &prompt, const std::vector<std::string> &options) {
    if (options.empty()) return "";
    std::cout << "\n=== " << prompt << " ===\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << (i + 1) << ". " << options[i] << "\n";
    }
    int choice = 0;
    while (true) {
        choice = get_menu_choice("Choose an option: ");
        if (choice < 1 || static_cast<size_t>(choice) > options.size()) {
            std::cout << "Invalid input\n";
            continue;
        }
        return options[static_cast<size_t>(choice - 1)];
    }
}

// Retrieves hospitals where at least one doctor with the given specialty works, based solely on criteria.
std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> 
get_hospitals_for_specialty(database_handler &db, const search_criteria &criteria) {
    std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> hospitals;
    const char *paramValues[4];
    paramValues[0] = criteria.region.c_str();
    paramValues[1] = criteria.settlement_type.c_str();
    paramValues[2] = criteria.settlement_name.c_str();
    paramValues[3] = criteria.specialty.c_str();
    
    const char *query =
        "SELECT DISTINCT h.hospital_id, h.full_name, h.street, h.house, u.phone "
        "FROM hospitals h "
        "JOIN doctors d ON h.hospital_id = ANY(d.hospital_ids) "
        "JOIN users u ON h.administrator_id = u.id "
        "WHERE h.region = $1 AND h.settlement_type = $2 AND h.settlement_name = $3 "
        "AND d.specialty = $4 "
        "ORDER BY h.full_name, h.street, h.house";
    
    PGresult *res = PQexecParams(db.get_connection(), query, 4, NULL, paramValues, NULL, NULL, 0);
    if (res && PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            int hospital_id = static_cast<int>(std::stoi(PQgetvalue(res, i, 0)));
            std::string full_name = PQgetvalue(res, i, 1);
            std::string street = PQgetvalue(res, i, 2);
            std::string house = PQgetvalue(res, i, 3);
            std::string admin_phone = PQgetvalue(res, i, 4);
            hospitals.push_back(std::make_tuple(hospital_id, full_name, street, house, admin_phone));
        }
    }
    if (res) PQclear(res);
    return hospitals;
}

// Presents a list of hospitals and returns the selected hospital id.
int select_hospital_for_doctor(database_handler &db, const search_criteria &criteria) {
    auto hospitals = get_hospitals_for_specialty(db, criteria);
    if (hospitals.empty()) {
        std::cout << "No hospitals found for the selected specialty\n";
        return -1;
    }
    if (hospitals.size() == 1) {
        return std::get<0>(hospitals[0]);
    }
    std::cout << "\n=== Hospitals List ===\n";
    for (size_t i = 0; i < hospitals.size(); ++i) {
        int hosp_id;
        std::string full_name, street, house, admin_phone;
        std::tie(hosp_id, full_name, street, house, admin_phone) = hospitals[i];
        std::cout << (i + 1) << ". " << full_name << ", street " << street 
                  << ", house " << house << ", admin phone: " << admin_phone << "\n";
    }
    int choice = 0;
    while (true) {
        choice = get_menu_choice("Choose a hospital: ");
        if (choice < 1 || static_cast<size_t>(choice) > hospitals.size()) {
            std::cout << "Invalid selection\n";
            continue;
        }
        return std::get<0>(hospitals[static_cast<size_t>(choice - 1)]);
    }
}

// Retrieves a list of doctors in the selected hospital with the given specialty.
std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> 
get_doctors_in_hospital(database_handler &db, const std::string &specialty, int hospital_id) {
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> doctors;
    const char *paramValues[2];
    paramValues[0] = specialty.c_str();
    std::string hosp_id_str = std::to_string(hospital_id);
    paramValues[1] = hosp_id_str.c_str();
    
    const char *query =
        "SELECT d.doctor_id, u.last_name, u.first_name, u.patronymic, d.experience "
        "FROM doctors d "
        "JOIN users u ON d.user_id = u.id "
        "WHERE d.specialty = $1 AND $2 = ANY(d.hospital_ids) "
        "ORDER BY u.last_name, u.first_name";
    
    PGresult *res = PQexecParams(db.get_connection(), query, 2, NULL, paramValues, NULL, NULL, 0);
    if (res && PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            std::string doctor_id = PQgetvalue(res, i, 0);
            std::string last_name = PQgetvalue(res, i, 1);
            std::string first_name = PQgetvalue(res, i, 2);
            std::string patronymic = PQgetvalue(res, i, 3);
            std::string experience = PQgetvalue(res, i, 4);
            doctors.push_back(std::make_tuple(doctor_id, last_name, first_name, patronymic, experience));
        }
    }
    if (res) PQclear(res);
    return doctors;
}

// Presents a list of doctors and returns the selected doctor id.
std::string select_doctor(const std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> &doctors) {
    if (doctors.empty()) return "";
    std::cout << "\n=== Doctors List ===\n";
    for (size_t i = 0; i < doctors.size(); ++i) {
        std::string doctor_id, last_name, first_name, patronymic, experience;
        std::tie(doctor_id, last_name, first_name, patronymic, experience) = doctors[i];
        std::cout << (i + 1) << ". " << last_name << " " << first_name << " " << patronymic
                  << ", experience: " << experience << " years\n";
    }
    int choice = 0;
    while (true) {
        choice = get_menu_choice("Choose a doctor: ");
        if (choice < 1 || static_cast<size_t>(choice) > doctors.size()) {
            std::cout << "Invalid selection\n";
            continue;
        }
        std::string doctor_id;
        std::tie(doctor_id, std::ignore, std::ignore, std::ignore, std::ignore) = doctors[static_cast<size_t>(choice - 1)];
        return doctor_id;
    }
}

// Displays the schedule for a selected doctor at the given hospital.
void display_doctor_schedule_for_patient(database_handler &db, const std::string &doctor_id, int hospital_id) {
    std::cout << "\n=== Doctor's Schedule ===\n";
    const char *paramValues[2];
    paramValues[0] = doctor_id.c_str();
    std::string hosp_id_str = std::to_string(hospital_id);
    paramValues[1] = hosp_id_str.c_str();
    
    const char *query =
        "SELECT appointment_date, appointment_time, cabinet_number, patient_id "
        "FROM records "
        "WHERE doctor_id = $1 AND hospital_id = $2 "
        "AND appointment_date BETWEEN CURRENT_DATE::date AND (CURRENT_DATE + INTERVAL '14 day')::date "
        "ORDER BY appointment_date, appointment_time";
    
    PGresult *res = PQexecParams(db.get_connection(), query, 2, NULL, paramValues, NULL, NULL, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "Error: Schedule not available. " << PQerrorMessage(db.get_connection()) << "\n";
        if (res) PQclear(res);
        return;
    }
    int rows = PQntuples(res);
    if (rows == 0) {
        std::cout << "No appointments for the next 14 days\n";
    } else {
        std::string current_date = "";
        for (int i = 0; i < rows; ++i) {
            std::string date = PQgetvalue(res, i, 0);
            std::string time = PQgetvalue(res, i, 1);
            std::string cabinet = PQgetvalue(res, i, 2);
            std::string patient_id = PQgetvalue(res, i, 3);
            if (date != current_date) {
                std::cout << "\n" << date << ":\n";
                current_date = date;
            }
            std::cout << time << ": cabinet " << cabinet;
            if (!patient_id.empty() && patient_id != "0")
                std::cout << ", occupied";
            else
                std::cout << ", free";
            std::cout << "\n";
        }
    }
    PQclear(res);
    std::cout << "\nAppointment scheduling temporarily unavailable\n";
}

void view_doctor_schedule_for_patient(database_handler &db) {
    // Step 1: Choose Region.
    std::vector<std::string> regions = get_unique_values(db, "SELECT DISTINCT region FROM hospitals ORDER BY region");
    std::string region = select_from_list("Region", regions);
    
    // Step 2: Choose Settlement Type.
    std::stringstream ss_type;
    ss_type << "SELECT DISTINCT settlement_type FROM hospitals WHERE region = '" << region << "' ORDER BY settlement_type";
    std::vector<std::string> settlement_types = get_unique_values(db, ss_type.str());
    std::string settlement_type = select_from_list("Settlement Type", settlement_types);
    
    // Step 3: Choose Settlement Name.
    std::stringstream ss_name;
    ss_name << "SELECT DISTINCT settlement_name FROM hospitals WHERE region = '" << region 
            << "' AND settlement_type = '" << settlement_type << "' ORDER BY settlement_name";
    std::vector<std::string> settlement_names = get_unique_values(db, ss_name.str());
    std::string settlement_name = select_from_list("Settlement Name", settlement_names);
    
    // Step 4: Choose Doctor Specialty.
    std::vector<std::string> specialties = get_unique_values(db, "SELECT DISTINCT specialty FROM doctors ORDER BY specialty");
    std::string specialty = select_from_list("Specialty", specialties);
    
    // Build search criteria.
    search_criteria criteria;
    criteria.region = region;
    criteria.settlement_type = settlement_type;
    criteria.settlement_name = settlement_name;
    criteria.specialty = specialty;
    
    // Step 5: Retrieve hospitals for the selected specialty.
    int hospital_id = select_hospital_for_doctor(db, criteria);
    if (hospital_id == -1) {
        return;
    }
    
    // Step 6: Retrieve doctors in the selected hospital with the chosen specialty.
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> doctors = 
        get_doctors_in_hospital(db, specialty, hospital_id);
    if (doctors.empty()) {
        std::cout << "No doctors found in the selected hospital\n";
        return;
    }
    
    std::string selected_doctor_id = select_doctor(doctors);
    if (selected_doctor_id.empty()) {
        std::cout << "Invalid doctor selection. Returning to patient cabinet.\n";
        return;
    }
    
    // Step 7: Display schedule for the selected doctor in the selected hospital.
    display_doctor_schedule_for_patient(db, selected_doctor_id, hospital_id);
}
