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
#include "../include/handlers/detach_doctor_from_hospital.hpp"
#include "../include/handlers/doctor_exists.hpp"
#include "../include/handlers/doctor_schedule.hpp"
#include "../include/handlers/edit_doctor_feedback.hpp"
#include "../include/handlers/get_doctor_average_ratings.hpp"
#include "../include/handlers/get_doctor_feedback_calculated.hpp"
#include "../include/handlers/get_doctor_feedback_items.hpp"
#include "../include/handlers/get_doctor_hospitals.hpp"
#include "../include/handlers/get_doctor_schedule_for_patient.hpp"
#include "../include/handlers/get_doctors.hpp"
#include "../include/handlers/get_doctors_for_patient.hpp"
#include "../include/handlers/get_hospital_full_names.hpp"
#include "../include/handlers/get_hospitals.hpp"
#include "../include/handlers/get_regions.hpp"
#include "../include/handlers/get_settlement_names.hpp"
#include "../include/handlers/get_settlement_types.hpp"
#include "../include/handlers/get_specialties.hpp"
#include "../include/handlers/get_users.hpp"
#include "../include/handlers/get_waitlist.hpp"
#include "../include/handlers/hospital_exists.hpp"
#include "../include/handlers/junior_admin_schedule.hpp"
#include "../include/handlers/login.hpp"
#include "../include/handlers/patient_appointments.hpp"
#include "../include/handlers/post_doctor_feedback.hpp"
#include "../include/handlers/registration.hpp"
#include "../include/handlers/search_doctors.hpp"

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
            try {
                json body = json::parse(req.body());
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
                    post_doctor_rating(body, res, db_handler);
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

                else {
                    handle_not_found(res);
                }
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

            else if (req.target().starts_with("/get_doctor_schedule_for_patient"
                     )) {
                try {
                    std::string url = std::string(req.target());

                    size_t doctor_pos = url.find("doctor_id=");
                    // size_t hospital_pos = url.find("&hospital_id=");

                    if (doctor_pos == std::string::npos) {
                        throw std::runtime_error("Missing required parameters");
                    }

                    int doctor_id = std::stoi(url.substr(doctor_pos + 10));
                    // int hospital_id = std::stoi(url.substr(hospital_pos +
                    // 13));

                    get_doctor_schedule_for_patient(doctor_id, res, db_handler);

                } catch (const std::exception &e) {
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = json{
                        {"success", false},
                        {"error",
                         std::string("Invalid request: ") +
                             e.what(
                             )}}.dump();
                }
            }

            else if (req.target().starts_with("/get_patient_appointments")) {
                try {
                    std::string url = std::string(req.target());

                    size_t query_start = url.find('?');
                    if (query_start == std::string::npos) {
                        throw std::runtime_error("Missing query parameters");
                    }

                    size_t pid_pos = url.find("patient_id=", query_start);
                    if (pid_pos == std::string::npos) {
                        throw std::runtime_error("Missing patient_id parameter"
                        );
                    }

                    pid_pos += 10;
                    size_t pid_end = url.find('&', pid_pos);
                    std::string pid_str = url.substr(
                        pid_pos, (pid_end == std::string::npos)
                                     ? std::string::npos
                                     : pid_end - pid_pos
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
                    std::cerr << "patinet_id" << patient_id << "\n";
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
            } else if (req.target().starts_with("/get_doctor_hospitals")) {
                try {
                    std::string url = std::string(req.target());

                    size_t param_pos = url.find("doctor_id=");
                    if (param_pos == std::string::npos) {
                        throw std::runtime_error("Missing doctor_id parameter");
                    }

                    size_t value_start = param_pos + 10;
                    size_t value_end = url.find('&', value_start);

                    std::string id_str =
                        (value_end == std::string::npos)
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

            else if (req.target().starts_with("/get_doctor_feedback_calculated"
                     )) {
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
            } else if (req.target().starts_with("/doctors_exist/")) {
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
                        res.body() =
                            R"({"success": false, "error": "Missing doctor_id"})";
                    }
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }

            else if (req.target().starts_with("/get_doctors_for_patient")) {
                try {
                    std::string url = std::string(req.target());
                    auto parse_param = [&](const std::string &key
                                       ) -> std::string {
                        size_t start = url.find(key + "=");
                        if (start == std::string::npos) {
                            return "";
                        }
                        start += key.length() + 1;
                        size_t end = url.find('&', start);
                        return end == std::string::npos
                                   ? url.substr(start)
                                   : url.substr(start, end - start);
                    };

                    json request_data;
                    request_data["region"] = parse_param("region");
                    request_data["settlement_type"] =
                        parse_param("settlement_type");
                    request_data["settlement_name"] =
                        parse_param("settlement_name");
                    request_data["specialty"] = parse_param("specialty");
                    request_data["hospital"] = parse_param("hospital");
                    request_data["doctor"] = parse_param("doctor");

                    get_doctors_for_patient(request_data, res, db_handler);
                } catch (const std::exception &e) {
                    json error;
                    error["success"] = false;
                    error["error"] =
                        std::string("Failed to process request: ") + e.what();

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

                    if (doc_pos == std::string::npos ||
                        adm_pos == std::string::npos) {
                        throw std::runtime_error("Missing required parameters");
                    }

                    int doctor_id = std::stoi(
                        url.substr(doc_pos + 10, adm_pos - doc_pos - 10)
                    );
                    int admin_id = std::stoi(url.substr(adm_pos + 10));

                    int hospital_id_admin = get_hospital_id_admin(admin_id);
                    bool is_valid =
                        check_doctor_hospital(doctor_id, hospital_id_admin);

                    json response = {
                        {"is_valid", is_valid},
                        {"admin_hospital_id", hospital_id_admin},
                        {"doctor_id", doctor_id}};

                    res.result(http::status::ok);
                    res.set(http::field::content_type, "application/json");
                    res.keep_alive(req.keep_alive());
                    res.body() = response.dump();
                    res.prepare_payload();
                    return;
                } catch (const std::exception &e) {
                    json error = {{"error", e.what()}, {"is_valid", false}};
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.keep_alive(req.keep_alive());
                    res.body() = error.dump();
                    res.prepare_payload();
                    return;
                }
            }

            else if (req.target() == "/junior_admin_schedule") {
                try {
                    junior_admin_schedule(json::object(), res, db_handler);
                } catch (const std::exception &e) {
                    handle_error(e, res);
                }
            }

            else if (req.target().starts_with("/get_waitlist")) {
                try {
                    std::string url = std::string(req.target());

                    auto parse_param = [&](const std::string &key) -> int {
                        size_t start = url.find(key + "=");
                        if (start == std::string::npos) {
                            throw std::runtime_error(
                                "Missing parameter: " + key
                            );
                        }
                        start += key.length() + 1;
                        size_t end = url.find('&', start);
                        std::string value =
                            (end == std::string::npos)
                                ? url.substr(start)
                                : url.substr(start, end - start);

                        try {
                            return std::stoi(value);
                        } catch (...) {
                            throw std::runtime_error(
                                "Invalid " + key + " format - must be integer"
                            );
                        }
                    };

                    int doctor_id = parse_param("doctor_id");
                    int junior_admin_id = parse_param("junior_admin_id");

                    get_waitlist(doctor_id, junior_admin_id, res, db_handler);

                } catch (const std::exception &e) {
                    json error;
                    error["success"] = false;
                    error["error"] =
                        std::string("Failed to process request: ") + e.what();

                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = error.dump();
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
                        std::string type =
                            get_user_type_by_phone(db_handler, phone);
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
            } else {
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

            if (req.target() == "/cancel_appointment") {
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
    res.prepare_payload();
}