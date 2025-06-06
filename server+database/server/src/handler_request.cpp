#include "../include/handler_request.hpp"
#include <openssl/sha.h>
#include <boost/algorithm/string.hpp>
#include <boost/beast/http.hpp>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <stdexcept>
#include "../include/check_doctor_hospital.hpp"
#include "../include/database.hpp"
#include "../include/get_hospital_id.hpp"
#include "../include/handlers/add_doctor.hpp"
#include "../include/handlers/add_hospital.hpp"
#include "../include/handlers/add_junior_admin.hpp"
#include "../include/handlers/add_patient_to_waitlist.hpp"
#include "../include/handlers/add_record_slot.hpp"
#include "../include/handlers/attach_doctor_to_hospital.hpp"
#include "../include/handlers/auth_handler.hpp"
#include "../include/handlers/book_appointment.hpp"
#include "../include/handlers/cancel_appointment.hpp"
#include "../include/handlers/cancel_waitlist.hpp"
#include "../include/handlers/delete_doctor_feedback.hpp"
#include "../include/handlers/delete_self_account.hpp"
#include "../include/handlers/delete_user_by_phone.hpp"
#include "../include/handlers/detach_doctor_from_hospital.hpp"
#include "../include/handlers/doctor_exists.hpp"
#include "../include/handlers/doctor_schedule.hpp"
#include "../include/handlers/edit_doctor_feedback.hpp"
#include "../include/handlers/get_admin_hospital.hpp"
#include "../include/handlers/get_cancelled_slots.hpp"
#include "../include/handlers/get_doctor_average_ratings.hpp"
#include "../include/handlers/get_doctor_feedback_calculated.hpp"
#include "../include/handlers/get_doctor_feedback_items.hpp"
#include "../include/handlers/get_doctor_hospitals.hpp"
#include "../include/handlers/get_doctor_profile.hpp"
#include "../include/handlers/get_doctor_schedule_for_patient.hpp"
#include "../include/handlers/get_doctors.hpp"
#include "../include/handlers/get_doctors_for_patient.hpp"
#include "../include/handlers/get_hospital_full_names.hpp"
#include "../include/handlers/get_hospitals.hpp"
#include "../include/handlers/get_junior_admin_profile.hpp"
#include "../include/handlers/get_patient_profile.hpp"
#include "../include/handlers/get_patient_waitlist.hpp"
#include "../include/handlers/get_regions.hpp"
#include "../include/handlers/get_senior_admin_profile.hpp"
#include "../include/handlers/get_settlement_names.hpp"
#include "../include/handlers/get_settlement_types.hpp"
#include "../include/handlers/get_specialties.hpp"
#include "../include/handlers/get_users.hpp"
#include "../include/handlers/get_waitlist.hpp"
#include "../include/handlers/get_waitlist_count.hpp"
#include "../include/handlers/hospital_exists.hpp"
#include "../include/handlers/junior_admin_schedule.hpp"
#include "../include/handlers/login.hpp"
#include "../include/handlers/patient_appointments.hpp"
#include "../include/handlers/post_doctor_feedback.hpp"
#include "../include/handlers/registration.hpp"
#include "../include/handlers/search_doctors.hpp"
#include "../include/handlers/edit_patient_profile.hpp"
#include "../include/handlers/edit_doctor_profile.hpp"
#include "../include/handlers/edit_senior_admin_profile.hpp"
#include "../include/handlers/edit_junior_admin_profile.hpp"
#include "../include/handlers/get_doctor_statistics.hpp"
#include "../include/handlers/send_notification.hpp"
#include "../include/cors_handler.hpp"

namespace http = boost::beast::http;
using json = nlohmann::json;

extern database_handler *global_db;

std::string base64_decode(const std::string &encoded) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    std::vector<int> vec(256, -1);
    for (int i = 0; i < 64; i++) {
        vec[base64_chars[i]] = i;
    }

    int val = 0, bits = -8;
    for (unsigned char c : encoded) {
        if (vec[c] == -1) {
            break;
        }
        val = (val << 6) + vec[c];
        bits += 6;
        if (bits >= 0) {
            decoded.push_back(char((val >> bits) & 0xFF));
            bits -= 8;
            val &= 0xFFF;
        }
    }
    return decoded;
}

void handle_not_found(http::response<http::string_body> &res) {
    res.result(http::status::not_found);
    res.set(http::field::content_type, "application/json");
    res.body() = R"({"error": "Route not found"})";
}

void handle_error(
    const std::exception &e,
    http::response<http::string_body> &res
) {
    res.result(http::status::internal_server_error);
    res.set(http::field::content_type, "application/json");
    res.body() = R"({"error": "Internal server error", "details": ")" +
                std::string(e.what()) + R"("})";
    add_cors_headers(res);
    std::cerr << "Exception: " << e.what() << std::endl;
}

bool extract_basic_credentials(
    const std::string &auth_header,
    std::string &phone,
    std::string &password
) {
    if (auth_header.find("Basic ") == 0) {
        std::string encoded = auth_header.substr(6);
        std::string decoded = base64_decode(encoded);
        size_t separator = decoded.find(':');
        if (separator != std::string::npos) {
            phone = decoded.substr(0, separator);
            password = decoded.substr(separator + 1);
            return true;
        }
    }
    return false;
}

void handle_request(
    const http::request<http::string_body> &req,
    http::response<http::string_body> &res,
    database_handler &db_handler
) {
    try {
        // Обрабатываем OPTIONS запросы (preflight запросы) в первую очередь
        if (req.method() == http::verb::options) {
            res.result(http::status::no_content);
            res.set(http::field::content_type, "text/plain");
            res.body() = "";
            add_cors_headers(res);
            res.prepare_payload();
            return; // Важно: выходим из функции после обработки OPTIONS
        }

        // Добавляем CORS-заголовки ко всем ответам
        add_cors_headers(res);

        if (req.method() == http::verb::post) {
            try {
                // Проверяем, что body не пустое
                if (req.body().empty()) {
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"error": "Empty request body", "success": false})";
                    return;
                }

                json body;
                try {
                    body = json::parse(req.body());
                } catch (const json::parse_error& e) {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                    std::cerr << "Request body: " << req.body() << std::endl;
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"error": "Invalid JSON format", "success": false})";
                    return;
                }

                if (req.target() == "/login") {
                    login(body, res, db_handler);
                }
                else if (req.target() == "/registration") {
                    registration(body, res, db_handler);
                }
                else if (req.target() == "/attach_doctor_to_hospital") {
                    attach_doctor_to_hospital(body, res, db_handler);
                }
                else if (req.target() == "/add_doctor") {
                    add_doctor(body, res, db_handler);
                }
                else if (req.target() == "/add_hospital") {
                    add_hospital(body, res, db_handler);
                }
                else if (req.target() == "/add_junior_admin") {
                    add_junior_admin(body, res, db_handler);
                }
                else if (req.target() == "/add_record_slot") {
                    add_record_slot(body, res, db_handler);
                }
                else if (req.target() == "/detach_doctor_from_hospital") {
                    detach_doctor_from_hospital(body, res, db_handler);
                }
                else if (req.target() == "/post_doctor_feedback") {
                    post_doctor_feedback(body, res, db_handler);
                }
                else if (req.target() == "/book_appointment") {
                    book_appointment(body, res, db_handler);
                }
                else if (req.target() == "/search_doctors") {
                    search_doctors(body, res, db_handler);
                }
                else if (req.target() == "/add_patient_to_waitlist") {
                    add_patient_to_waitlist(body, res, db_handler);
                }
                else if (req.target() == "/cancel_appointment_from_waitlist") {
                    cancel_waitlist(body, res, db_handler);
                }
                else if (req.target() == "/get_doctor_statistics") {
                    get_doctor_statistics(body, res, db_handler);
                }
                else if (req.target() == "/send_notification") {
                    send_notification_handler(body, res, db_handler);
                }
                else {
                    handle_not_found(res);
                }
            } catch (const json::parse_error& e) {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "Invalid JSON format", "success": false})";
            } catch (const std::exception &e) {
                handle_error(e, res);
            }
        }
        else if (req.method() == http::verb::get) {
            if (req.target() == "/get_doctors") {
                try {
                    get_doctors_table(json::object(), res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/get_hospitals") {
                try {
                    get_hospitals_table(json::object(), res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/get_users") {
                try {
                    get_users_table(json::object(), res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/get_doctor_schedule_for_patient")) {
                try {
                    std::string url = std::string(req.target());
                    size_t doctor_pos = url.find("doctor_id=");

                    if (doctor_pos == std::string::npos) {
                        throw std::runtime_error("Missing required parameters");
                    }

                    int doctor_id = std::stoi(url.substr(doctor_pos + 10));
                    get_doctor_schedule_for_patient(doctor_id, res, db_handler);

                } catch (const std::exception &e) {
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = json{
                        {"success", false},
                        {"error", std::string("Invalid request: ") + e.what()}}.dump();
                }
            }
            else if (req.target().starts_with("/get_patient_appointments")) {
                try {
                    std::string url = std::string(req.target());
                    size_t pid_pos = url.find("patient_id=");
                    if (pid_pos == std::string::npos) {
                        throw std::runtime_error("Missing patient_id parameter");
                    }

                    pid_pos += 10;
                    size_t pid_end = url.find('&', pid_pos);
                    std::string pid_str = url.substr(
                        pid_pos, 
                        (pid_end == std::string::npos) ? std::string::npos : pid_end - pid_pos
                    );

                    pid_str.erase(
                        std::remove_if(
                            pid_str.begin(), pid_str.end(),
                            [](char c) { return !std::isdigit(c); }
                        ),
                        pid_str.end()
                    );

                    if (pid_str.empty()) {
                        throw std::invalid_argument("Empty patient_id");
                    }

                    int patient_id = std::stoi(pid_str);
                    patient_appointments(patient_id, res, db_handler);

                } catch (const std::invalid_argument &) {
                    json error = {
                        {"success", false},
                        {"error", "Patient ID must be a number"}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
                } catch (const std::exception &e) {
                    json error = {{"success", false}, {"error", e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
                }
            } 
            else if (req.target().starts_with("/get_doctor_hospitals")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("doctor_id=");
                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing doctor_id parameter");
                    }

                    size_t value_start = param_pos + 10;
                    size_t value_end = url.find('&', value_start);
                    std::string id_str = (value_end == std::string::npos)
                        ? url.substr(value_start)
                        : url.substr(value_start, value_end - value_start);

                    int doctor_id = std::stoi(id_str);
                    get_doctor_hospitals(doctor_id, res, db_handler);

                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/get_regions") {
                get_regions(res, db_handler);
            }
            else if (req.target() == "/get_doctor_average_ratings") {
                get_doctor_average_ratings(res, db_handler);
            }
            else if (req.target() == "/get_settlement_types") {
                get_settlement_types(res, db_handler);
            }
            else if (req.target() == "/get_settlement_names") {
                get_settlement_names(res, db_handler);
            }
            else if (req.target() == "/get_hospital_full_names") {
                get_hospital_full_names(res, db_handler);
            }
            else if (req.target() == "/get_specialties") {
                get_specialties(res, db_handler);
            }
            else if (req.target().starts_with("/get_doctor_feedback_items")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("?doctor_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing doctor_id parameter");
                    }

                    int doctor_id = std::stoi(url.substr(param_pos + 11));
                    json request_data = {{"doctor_id", doctor_id}};
                    get_doctor_rating_items(request_data, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_doctor_feedback_calculated")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("?doctor_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing doctor_id parameter");
                    }

                    int doctor_id = std::stoi(url.substr(param_pos + 11));
                    json request_data = {{"doctor_id", doctor_id}};
                    get_doctor_rating_calculated(request_data, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            } 
            else if (req.target().starts_with("/doctors_exist/")) {
                try {
                    std::string target = std::string(req.target());
                    int doctor_id = std::stoi(target.substr(16));
                    doctor_exists(doctor_id, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/hospitals_exist/")) {
                try {
                    std::string target = std::string(req.target());
                    int hospital_id = std::stoi(target.substr(17));
                    hospital_exists(hospital_id, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/get_doctor_schedule")) {
                try {
                    std::string target = std::string(req.target());
                    size_t pos = target.find("doctor_id=");
                    if (pos != std::string::npos) {
                        int doctor_id = std::stoi(target.substr(pos + 10));
                        json request_data;
                        request_data["doctor_id"] = doctor_id;
                        get_doctor_schedule(request_data, res, db_handler);
                    } else {
                        res.result(http::status::bad_request);
                        res.set(http::field::content_type, "application/json");
                        res.body() = R"({"success": false, "error": "Missing doctor_id"})";
                    }
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/get_doctors_for_patient")) {
                try {
                    std::string url = std::string(req.target());
                    auto parse_param = [&](const std::string &key) -> std::string {
                        size_t start = url.find(key + "=");
                        if (start == std::string::npos) return "";
                        start += key.length() + 1;
                        size_t end = url.find('&', start);
                        return end == std::string::npos
                                ? url.substr(start)
                                : url.substr(start, end - start);
                    };

                    json request_data;
                    request_data["region"] = parse_param("region");
                    request_data["settlement_type"] = parse_param("settlement_type");
                    request_data["settlement_name"] = parse_param("settlement_name");
                    request_data["specialty"] = parse_param("specialty");
                    request_data["hospital"] = parse_param("hospital");
                    request_data["doctor"] = parse_param("doctor");

                    get_doctors_for_patient(request_data, res, db_handler);
                } catch (const std::exception &e) {
                    json error;
                    error["success"] = false;
                    error["error"] = std::string("Failed to process request: ") + e.what();
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
                }
            }
            else if (req.target().starts_with("/check_doctor_admin_hospital")) {
                try {
                    std::string url = std::string(req.target());
                    auto doc_pos = url.find("doctor_id=");
                    auto adm_pos = url.find("&admin_id=");

                    if (doc_pos == std::string::npos || adm_pos == std::string::npos) {
                        throw std::runtime_error("Missing required parameters");
                    }

                    int doctor_id = std::stoi(url.substr(doc_pos + 10, adm_pos - doc_pos - 10));
                    int admin_id = std::stoi(url.substr(adm_pos + 10));

                    int hospital_id_admin = get_hospital_id_admin(admin_id);
                    bool is_valid = check_doctor_hospital(doctor_id, hospital_id_admin);

                    json response = {
                        {"is_valid", is_valid},
                        {"admin_hospital_id", hospital_id_admin},
                        {"doctor_id", doctor_id}};

                    res.result(http::status::ok);
                    res.set(http::field::content_type, "application/json");
                    res.keep_alive(req.keep_alive());
                    res.body() = response.dump();
                    res.prepare_payload();
                } catch (const std::exception &e) {
                    json error = {{"error", e.what()}, {"is_valid", false}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.keep_alive(req.keep_alive());
                    res.body() = error.dump();
                    res.prepare_payload();
                }
            }
            else if (req.target() == "/junior_admin_schedule") {
                try {
                    junior_admin_schedule(json::object(), res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/get_user_id")) {
                try {
                    json out;
                    std::string target = std::string(req.target());
                    auto pos = target.find("phone=");
                    if (pos == std::string::npos) {
                        res.result(http::status::bad_request);
                        out["error"] = "Missing phone parameter";
                    } else {
                        std::string phone = target.substr(pos + 6);
                        int id = get_user_id_by_phone(db_handler, phone);
                        if (id < 0) {
                            res.result(http::status::not_found);
                            out["error"] = "User not found";
                        } else {
                            res.result(http::status::ok);
                            out["id"] = id;
                        }
                    }
                    res.set(http::field::content_type, "application/json");
                    res.body() = out.dump();
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/get_user_type")) {
                try {
                    json out;
                    std::string target = std::string(req.target());
                    auto pos = target.find("phone=");
                    if (pos == std::string::npos) {
                        res.result(http::status::bad_request);
                        out["error"] = "Missing phone parameter";
                    } else {
                        std::string phone = target.substr(pos + 6);
                        std::string type = get_user_type_by_phone(db_handler, phone);
                        if (type.empty()) {
                            res.result(http::status::not_found);
                            out["error"] = "User not found";
                        } else {
                            res.result(http::status::ok);
                            out["user_type"] = type;
                        }
                    }
                    res.set(http::field::content_type, "application/json");
                    res.body() = out.dump();
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/get_cancelled_slots")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("doctor_id=");

                    if (param_pos == std::string::npos) {
                        json error_response = {
                            {"success", false},
                            {"error", "Missing doctor_id parameter"}};
                        res.result(http::status::bad_request);
                        res.set(http::field::content_type, "application/json");
                        res.body() = error_response.dump();
                        return;
                    }

                    int doctor_id;
                    try {
                        size_t value_start = param_pos + 10;
                        size_t value_end = url.find('&', value_start);
                        std::string id_str = (value_end == std::string::npos)
                            ? url.substr(value_start)
                            : url.substr(value_start, value_end - value_start);

                        doctor_id = std::stoi(id_str);
                    } catch (const std::exception &e) {
                        json error_response = {
                            {"success", false},
                            {"error", "Invalid doctor_id format - must be integer"}};
                        res.result(http::status::bad_request);
                        res.set(http::field::content_type, "application/json");
                        res.body() = error_response.dump();
                        return;
                    }

                    get_cancelled_slots(doctor_id, res, db_handler);
                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Internal server error: ") + e.what()}};
                    res.result(http::status::internal_server_error);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_waitlist_count")) {
                try {
                    std::string url = std::string(req.target());
                    auto parse_param = [&](const std::string &key) -> int {
                        size_t start = url.find(key + "=");
                        if (start == std::string::npos) {
                            throw std::runtime_error("Missing parameter: " + key);
                        }
                        start += key.length() + 1;
                        size_t end = url.find('&', start);
                        std::string value = (end == std::string::npos)
                                ? url.substr(start)
                                : url.substr(start, end - start);
                        return std::stoi(value);
                    };

                    int doctor_id = parse_param("doctor_id");
                    get_waitlist_count(doctor_id, res, db_handler);
                } catch (const std::exception &e) {
                    json error;
                    error["success"] = false;
                    error["error"] = std::string("Failed to process request: ") + e.what();
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
                }
            }
            else if (req.target().starts_with("/get_waitlist")) {
                try {
                    std::string url = std::string(req.target());
                    auto parse_param = [&](const std::string &key) -> int {
                        size_t start = url.find(key + "=");
                        if (start == std::string::npos) {
                            throw std::runtime_error("Missing parameter: " + key);
                        }
                        start += key.length() + 1;
                        size_t end = url.find('&', start);
                        std::string value = (end == std::string::npos)
                                ? url.substr(start)
                                : url.substr(start, end - start);
                        return std::stoi(value);
                    };

                    int doctor_id = parse_param("doctor_id");
                    int junior_admin_id = parse_param("junior_admin_id");
                    get_waitlist(doctor_id, junior_admin_id, res, db_handler);
                } catch (const std::exception &e) {
                    json error;
                    error["success"] = false;
                    error["error"] = std::string("Failed to process request: ") + e.what();
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
                }
            }
            else if (req.target().starts_with("/get_patient_profile")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("user_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing user_id parameter");
                    }

                    int user_id = std::stoi(url.substr(param_pos + 8));
                    get_patient_profile(user_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_doctor_profile")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("user_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing user_id parameter");
                    }

                    int user_id = std::stoi(url.substr(param_pos + 8));
                    get_doctor_profile(user_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_junior_admin_profile")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("user_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing user_id parameter");
                    }

                    int user_id = std::stoi(url.substr(param_pos + 8));
                    get_junior_admin_profile(user_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_senior_admin_profile")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("user_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing user_id parameter");
                    }

                    int user_id = std::stoi(url.substr(param_pos + 8));
                    get_senior_admin_profile(user_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_patient_waitlist")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("patient_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing patient_id parameter");
                    }

                    int patient_id = std::stoi(url.substr(param_pos + 11));
                    get_patient_waitlist(patient_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else if (req.target().starts_with("/get_admin_hospital")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("admin_id=");

                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing admin_id parameter");
                    }

                    int admin_id = std::stoi(url.substr(param_pos + 9));
                    get_admin_hospital(admin_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else {
                handle_not_found(res);
            }
        }
        else if (req.method() == http::verb::delete_) {
            if (req.target() == "/delete_doctor_feedback") {
                try {
                    json body = json::parse(req.body());
                    delete_doctor_feedback(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target().starts_with("/delete_self_account")) {
                try {
                    std::string url = std::string(req.target());
                    size_t param_pos = url.find("user_id=");

                    if (param_pos == std::string::npos) {
                        json error = {
                            {"success", false},
                            {"error", "Missing user_id parameter"}};
                        res.result(http::status::bad_request);
                        res.body() = error.dump();
                        return;
                    }

                    int user_id = std::stoi(url.substr(param_pos + 8));
                    delete_self_account(user_id, res, db_handler);
                } catch (const std::exception &e) {
                    json error = {
                        {"success", false},
                        {"error", std::string("Invalid request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.body() = error.dump();
                }
            }
            else if (req.target().starts_with("/delete_user_by_phone")) {
                try {
                    std::string url = std::string(req.target());
                    auto parse_string_param = [&](const std::string &key) -> std::string {
                        size_t start = url.find(key + "=");
                        if (start == std::string::npos) {
                            throw std::runtime_error("Missing parameter: " + key);
                        }
                        start += key.length() + 1;
                        size_t end = url.find('&', start);
                        return (end == std::string::npos)
                                ? url.substr(start)
                                : url.substr(start, end - start);
                    };

                    std::string phone = parse_string_param("phone");
                    phone.erase(std::remove(phone.begin(), phone.end(), '"'), phone.end());
                    phone.erase(std::remove(phone.begin(), phone.end(), ' '), phone.end());

                    if (phone.empty()) {
                        throw std::runtime_error("Phone number cannot be empty");
                    }

                    delete_user_by_phone(phone, res, db_handler);
                } catch (const std::exception &e) {
                    json error_response = {
                        {"success", false},
                        {"error", std::string("Bad request: ") + e.what()}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error_response.dump();
                }
            }
            else {
                handle_not_found(res);
            }
        }
        else if (req.method() == http::verb::patch) {
            if (req.target() == "/edit_doctor_feedback") {
                try {
                    json body = json::parse(req.body());
                    edit_doctor_feedback(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/edit_doctor_profile") {
                try {
                    json body = json::parse(req.body());
                    edit_doctor_profile(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/edit_junior_admin_profile") {
                try {
                    json body = json::parse(req.body());
                    edit_junior_admin_profile(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/edit_senior_admin_profile") {
                try {
                    json body = json::parse(req.body());
                    edit_senior_admin_profile(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/edit_patient_profile") {
                try {
                    json body = json::parse(req.body());
                    edit_patient_profile(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else if (req.target() == "/cancel_appointment") {
                try {
                    json body = json::parse(req.body());
                    cancel_appointment(body, res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }
            else {
                handle_not_found(res);
            }
        }
        else {
            res.result(http::status::method_not_allowed);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Method not allowed"})";
        }
    } catch (const std::exception &e) {
        handle_error(e, res);
    }
    
    // Гарантируем, что CORS-заголовки добавлены даже в случае ошибок
    add_cors_headers(res);
    res.prepare_payload();
}
