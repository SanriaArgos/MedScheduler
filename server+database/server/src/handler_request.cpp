#include "../include/handler_request.hpp"
#include "../include/database.hpp"

#include "nlohmann/json.hpp"
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp> 

namespace http = boost::beast::http;
using json = nlohmann::json;

#include "../include/handlers/login.hpp"
#include "../include/handlers/registration.hpp"
#include "../include/handlers/add_hospital.hpp"
#include "../include/handlers/add_junior_admin.hpp"
#include "../include/handlers/add_doctor.hpp"
#include "../include/handlers/add_record_slot.hpp"
#include "../include/handlers/display_doctors.hpp"
#include "../include/handlers/display_hospitals.hpp"
#include "../include/handlers/display_users.hpp"
#include "../include/handlers/doctor_hospital_managment.hpp"
#include "../include/handlers/doctor_schedule.hpp"
#include "../include/handlers/junior_admin_schedule.hpp"
#include "../include/handlers/patient_schedule.hpp"

std::string base64_decode(const std::string &encoded) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    std::vector<int> vec(256, -1);
    for (int i = 0; i < 64; i++) vec[base64_chars[i]] = i;

    int val = 0, bits = -8;
    for (unsigned char c : encoded) {
        if (vec[c] == -1) break;
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

void handle_error(const std::exception &e, http::response<http::string_body> &res) {
    res.result(http::status::internal_server_error);
    res.set(http::field::content_type, "application/json");
    res.body() = R"({"error": "Internal server error", "details": ")" + std::string(e.what()) + R"("})";
    std::cerr << "Exception: " << e.what() << std::endl;
}

bool extract_basic_credentials(const std::string &auth_header, std::string &phone, std::string &password) {
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
            }
            else if (req.target() == "/registration") {
                registration(body, res, db_handler);
            } 
            else if (req.target() == "/add_doctor") {
                json body = json::parse(req.body());
                add_doctor(body, res, db_handler);
            }
            else if (req.target() == "/add_hospital") {
                add_hospital(body, res, db_handler);
            } else if (req.target() == "/add_junior_admin") {
                add_junior_admin(body, res, db_handler);
            } else if (req.target() == "/add_record_slot") {
                add_record_slot(body, res, db_handler);
            } else if (req.target() == "/add_hospital_to_doctor") {
                add_hospital_to_doctor(body, res, db_handler);
            } else if (req.target() == "/remove_hospital_from_doctor") {
                remove_hospital_from_doctor(body, res, db_handler);
            } else {
                handle_not_found(res);
            }
        } else if (req.method() == http::verb::get) {
            if (req.target() == "/display_doctors") {
                display_doctors_table(json::object(), res, db_handler);  // Пустой JSON, так как данные не требуются
            } else if (req.target() == "/display_hospitals") {
                display_hospitals_table(json::object(), res, db_handler);  // Пустой JSON, так как данные не требуются
            } else if (req.target() == "/display_users") {
                display_users_table(json::object(), res, db_handler);  // Пустой JSON, так как данные не требуются
            } else if (req.target() == "/display_doctor_schedule") {
                display_doctor_schedule(json::object(), res, db_handler);  // Пустой JSON, так как данные не требуются
            } else if (req.target() == "/junior_admin_schedule") {
                junior_admin_schedule(json::object(), res, db_handler);  // Пустой JSON, так как данные не требуются
            } else if (req.target() == "/view_doctor_schedule_for_patient") {
                view_doctor_schedule_for_patient(json::object(), res, db_handler);  // Пустой JSON, так как данные не требуются
            }
            else if (req.target().starts_with("/get_user_id")) {
                json out;
                auto pos = req.target().find("phone=");
                if (pos == std::string::npos) {
                    res.result(http::status::bad_request);
                    out["error"] = "Missing phone parameter";
                } else {
                    std::string phone{ req.target().substr(pos + 6) };
                    int id = db_handler.get_user_id_by_phone(phone);
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
                    std::string phone{ req.target().substr(pos + 6) };
                    std::string type = db_handler.get_user_type_by_phone(phone);
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
            }
            else {
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

// Остальные функции (base64_decode, extract_basic_credentials, handle_not_found, handle_error) остаются без изменений