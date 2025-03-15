#include "auth.hpp"
#include <cctype>

namespace Auth {

std::string trim_whitespace(const std::string &str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? ""
                                        : str.substr(start, end - start + 1);
}

bool validate_phone(const std::string &phone) {
    if (phone.empty() || phone[0] != '+') {
        return false;
    }
    std::string digits = phone.substr(1);
    if (digits.size() != 11) {
        return false;
    }
    if (digits[0] != '7') {
        return false;
    }
    for (char c : digits) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

}  // namespace Auth