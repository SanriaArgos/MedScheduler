#ifndef UTILS_H
#define UTILS_H
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
bool is_valid_password(const QString &str);
bool is_latin_or_dash(const QString &str);
bool is_validated_phone(const QString &str);
bool is_number(const QString &str);
bool is_valid_date(const QString &str);
bool is_valid_time(const QString &str);
#endif  // UTILS_H
