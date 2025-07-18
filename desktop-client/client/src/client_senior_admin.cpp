#include "client_senior_admin.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "common_for_all.hpp"

namespace senior_admin {

using json = nlohmann::json;

senior_admin_client::senior_admin_client(int admin_id) : admin_id(admin_id){};

void senior_admin_client::add_junior_admin(const json &json_data) {
    std::string url = base_url + "/add_junior_admin";
    std::string response = send_post_request(url, json_data);
}

void senior_admin_client::add_hospital(const json &json_data) {
    std::string url = base_url + "/add_hospital";
    std::string response = send_post_request(url, json_data);
}

json senior_admin_client::get_users_table() {
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

json senior_admin_client::get_hospitals_table() {
    std::string url = base_url + "/get_hospitals";
    std::string response = send_get_request(url);

    try {
        json hospitals = json::parse(response);
        return hospitals;
    } catch (const std::exception &e) {
        std::cerr << "Error fetching hospitals table: " << e.what()
                  << std::endl;
        return json();
    }
}

json senior_admin_client::delete_user_by_phone(const std::string &user_phone) {
    std::string url =
        base_url + "/delete_user_by_phone?user_phone=" + user_phone;
    std::string response = send_get_request(url);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        std::cerr << "Error fetching hospitals table: " << e.what()
                  << std::endl;
        return json();
    }
}

json senior_admin_client::edit_senior_admin_profile(const json &request_data) {
    std::string url = base_url + "/edit_senior_admin_profile";
    std::string response = send_patch_request(url, request_data);

    try {
        return json::parse(response);
    } catch (const std::exception &e) {
        return {
            {"success", false},
            {"error", std::string("Invalid JSON from server: ") + e.what()}};
    }
}

json get_junior_admin_profile_by_id(int user_id);

json senior_admin_client::get_senior_admin_profile_by_id(int user_id) {
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

}  // namespace senior_admin
