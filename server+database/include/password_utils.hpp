#ifndef PASSWORD_UTILS_HPP_
#define PASSWORD_UTILS_HPP_

#include <openssl/sha.h>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

class PasswordUtils {
public:
    static std::string generate_Salt(size_t length);
    static std::string
    hash_Password(const std::string &password, const std::string &salt);
};

#endif  // PASSWORD_UTILS_HPP