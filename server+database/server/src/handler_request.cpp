#include "../include/handler_request.hpp"
#include "../include/handlers/auth_handler.hpp"
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
#include "../include/database.hpp"
#include "nlohmann/json.hpp"

#include "../include/database.hpp"
#include "../include/get_hospital_id.hpp"
#include "../include/check_doctor_hospital.hpp" 
#include "../include/handlers/add_doctor.hpp"
#include "../include/handlers/add_hospital.hpp"
#include "../include/handlers/add_junior_admin.hpp"
#include "../include/handlers/add_record_slot.hpp"
#include "../include/handlers/attach_doctor_to_hospital.hpp"
#include "../include/handlers/doctor_exists.hpp"
#include "../include/handlers/detach_doctor_from_hospital.hpp"
#include "../include/handlers/doctor_schedule.hpp"
#include "../include/handlers/get_doctors.hpp"
#include "../include/handlers/get_hospitals.hpp"
#include "../include/handlers/get_users.hpp"
#include "../include/handlers/hospital_exists.hpp"
#include "../include/handlers/junior_admin_schedule.hpp"
#include "../include/handlers/login.hpp"
#include "../include/handlers/patient_schedule.hpp"
#include "../include/handlers/registration.hpp"
#include "../include/handlers/post_doctor_feedback.hpp"

extern database_handler* global_db;

namespace http = boost::beast::http;
using json = nlohmann::json;

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
        if (req.method() == http::verb::post) {
            json body = json::parse(req.body());
            if (req.target() == "/login") {
                login(body, res, db_handler);
            } else if (req.target() == "/registration") {
                registration(body, res, db_handler);
            } else if (req.target() == "/attach_doctor_to_hospital") {
                attach_doctor_to_hospital(body, res, db_handler);
            } else if (req.target() == "/add_doctor") {
                add_doctor(body, res, db_handler);
            } else if (req.target() == "/add_hospital") {
                add_hospital(body, res, db_handler);
            } else if (req.target() == "/add_junior_admin") {
                add_junior_admin(body, res, db_handler);
            } else if (req.target() == "/add_record_slot") {
                add_record_slot(body, res, db_handler);
            } else if (req.target() == "/detach_doctor_from_hospital") {
                detach_hospital_from_doctor(body, res, db_handler);
            } else if (req.target() == "/view_doctor_schedule_for_patient") {
                view_doctor_schedule_for_patient(body, res, db_handler);
            } else if (req.target() == "/post_doctor_rating") {
                post_doctor_rating(body, res, db_handler);
            } else {
                handle_not_found(res);
            }
        } else if (req.method() == http::verb::get) {
            if (req.target() == "/get_doctors") {
                get_doctors_table(
                    json::object(), res, db_handler
                ); 
            } else if (req.target() == "/get_hospitals") {
                get_hospitals_table(
                    json::object(), res, db_handler
                ); 
            } else if (req.target() == "/get_users") {
                get_users_table(
                    json::object(), res, db_handler
                ); 
            } else if (req.target().starts_with("/doctors_exist/")) {
                std::string id_str =
                    std::string(req.target())
                        .substr(std::string("/doctors_exist/").length());
                int doctor_id = std::stoi(id_str);
                doctor_exists(doctor_id, res, db_handler);
            } else if (req.target().starts_with("/hospitals_exist/")) {
                std::string id_str =
                    std::string(req.target())
                        .substr(std::string("/hospitals_exist/").length());
                int hospital_id = std::stoi(id_str);
                hospital_exists(hospital_id, res, db_handler);
            } else if (req.target().starts_with("/get_doctor_schedule")) {
                std::string target = std::string(req.target());
                
                size_t doctor_id_start = target.find("doctor_id=");
                
                if (doctor_id_start != std::string::npos) {
                    doctor_id_start += 10; // Длина "doctor_id="
                    
                    size_t doctor_id_end = target.find('&', doctor_id_start);
                    
                    std::string doctor_id_str = (doctor_id_end == std::string::npos) 
                        ? target.substr(doctor_id_start) 
                        : target.substr(doctor_id_start, doctor_id_end - doctor_id_start);
                    std::cerr << "doctor_id" << doctor_id_str << "\n";
                    try {
                        int doctor_id = std::stoi(doctor_id_str);
                        
                        json request_data;
                        request_data["doctor_id"] = doctor_id;
                        get_doctor_schedule(request_data, res, db_handler);
                        
                    } catch (const std::exception &e) {
                        json error;
                        error["success"] = false;
                        error["error"] = "Invalid doctor_id format: must be integer";
                        
                        res.result(http::status::bad_request);
                        res.set(http::field::content_type, "application/json");
                        res.body() = error.dump();
                    }
                } else {
                    json error;
                    error["success"] = false;
                    error["error"] = "Missing doctor_id";

                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
                }
            } 
            else if (req.target().starts_with("/check_doctor_admin_hospital")) {
    try {
        const std::string &url = req.target().to_string();
        size_t doctor_pos = url.find("doctor_id=");
        size_t admin_pos = url.find("&admin_id=");

        if (doctor_pos == std::string::npos || admin_pos == std::string::npos) {
            throw std::runtime_error("Missing required parameters in URL");
        }

        std::string doctor_id_str = url.substr(doctor_pos + 10, admin_pos - (doctor_pos + 10));
        std::string admin_id_str = url.substr(admin_pos + 10);

        std::cerr << "doctor_id_str admin_id_str: " << doctor_id_str << " " << admin_id_str << "\n";

        size_t amp_pos = admin_id_str.find('&');
        if (amp_pos != std::string::npos) {
            admin_id_str = admin_id_str.substr(0, amp_pos);
        }

        int doctor_id = std::stoi(doctor_id_str);
        int admin_id = std::stoi(admin_id_str);

        int hospital_id_admin = get_hospital_id_admin(admin_id);

        bool is_valid = check_doctor_hospital(doctor_id, hospital_id_admin);

        json response;
        response["is_valid"] = is_valid;
        response["admin_hospital_id"] = hospital_id_admin;
        response["doctor_id"] = doctor_id;

        if (!is_valid) {
            std::cerr << "Error: Doctor and admin are not associated with the same hospital\n";
        }

        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.result(http::status::ok);
        res.body() = response.dump();
        res.prepare_payload();
        return;
    } catch (const std::exception &e) {
        std::cerr << "Error checking hospital association: " << e.what() << std::endl;

        json error_response;
        error_response["error"] = e.what();
        error_response["is_valid"] = false;

        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.result(http::status::bad_request);
        res.body() = error_response.dump();
        res.prepare_payload();
        return;
    }
}
            else if (req.target() == "/junior_admin_schedule") {
                junior_admin_schedule(
                    json::object(), res, db_handler
                ); 
            } else if (req.target().starts_with("/get_user_id")) {
                json out;
                auto pos = req.target().find("phone=");
                if (pos == std::string::npos) {
                    res.result(http::status::bad_request);
                    out["error"] = "Missing phone parameter";
                } else {
                    std::string phone{req.target().substr(pos + 6)};
                    int id = get_user_id_by_phone(db_handler, phone);
                    if (id < 0) {
                        res.result(http::status::not_found);
                        out["error"] = "User not found";
                        std::cerr << id << std::endl;
                    } else {
                        res.result(http::status::ok);
                        out["id"] = id;
                        std::cerr << id << std::endl;
                    }
                }
                res.set(http::field::content_type, "application/json");
                res.body() = out.dump();
            } else if (req.target().starts_with("/get_user_type")) {
                json out;
                auto pos = req.target().find("phone=");
                if (pos == std::string::npos) {
                    res.result(http::status::bad_request);
                    out["error"] = "Missing phone parameter";
                } else {
                    std::string phone{req.target().substr(pos + 6)};
                    std::string type = get_user_type_by_phone(db_handler, phone);
                    if (type.empty()) {
                        res.result(http::status::not_found);
                        out["error"] = "User not found";
                        std::cerr << type << std::endl;
                    } else {
                        res.result(http::status::ok);
                        out["user_type"] = type;
                        std::cerr << type << std::endl;
                    }
                }
                res.set(http::field::content_type, "application/json");
                res.body() = out.dump();
            } else {
                handle_not_found(res);
            }
        } else {
            res.result(http::status::method_not_allowed);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Method not allowed"})";
        }
    } catch (const std::exception &e) {
        handle_error(e, res);
    }
    res.prepare_payload();
}
