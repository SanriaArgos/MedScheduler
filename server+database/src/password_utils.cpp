#include "password_utils.hpp"

std::string PasswordUtils::generate_Salt(size_t length) {
    static const char char_set[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist(0, (int)(sizeof(char_set) - 2));
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt.push_back(char_set[dist(engine)]);
    }
    return salt;
}

std::string PasswordUtils::hash_Password(const std::string &password, const std::string &salt) {
    std::string to_hash = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH] = {0};
    SHA256(reinterpret_cast<const unsigned char*>(to_hash.c_str()),
           to_hash.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }
    return ss.str();
}