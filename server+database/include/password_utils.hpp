#ifndef PASSWORD_UTILS_HPP
#define PASSWORD_UTILS_HPP

#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

class PasswordUtils {
public:
    static std::string generate_Salt(size_t length);
    static std::string hash_Password(const std::string &password, const std::string &salt);
};

#endif // PASSWORD_UTILS_HPP