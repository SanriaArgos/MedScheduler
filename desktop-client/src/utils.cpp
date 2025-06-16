#include "utils.h"
#include <QRegularExpression>
#include <QString>
#include <cctype>
#include <string>

bool is_latin_or_dash(const QString &str) {
    std::string s = str.toStdString();
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '-'))) {
            return false;
        }
    }
    return true;
}

bool is_valid_password(const QString &str) {
    std::string s = str.toStdString();
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (!std::isprint(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool is_validated_phone(const QString &str) {
    std::string phone = str.toStdString();
    if (phone.size() != 12 || phone[0] != '+' || phone[1] != '7') {
        return false;
    }
    for (size_t i = 2; i < phone.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(phone[i]))) {
            return false;
        }
    }
    return true;
}

bool is_number(const QString &str) {
    std::string s = str.toStdString();
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (!(c >= '0' && c <= '9')) {
            return false;
        }
    }
    return true;
}

bool is_valid_date(const QString &str) {
    QRegularExpression regex("^\\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$"
    );
    if (regex.match(str).hasMatch()) {
        return true;
    }
    return false;
}

bool is_valid_time(const QString &str) {
    QRegularExpression regex("^([01]?[0-9]|2[0-3]):[0-5][0-9]$");
    if (regex.match(str).hasMatch()) {
        return true;
    }
    return false;
}

