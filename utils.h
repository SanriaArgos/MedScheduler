#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
bool is_valid_password(const QString &str);
bool is_latin_or_dash(const QString &str);
bool is_validated_phone(const QString &str);
bool is_number(const QString &str);
bool is_valid_date(const QString &str);
bool is_valid_time(const QString &str);
void add_json_to_users(const QJsonObject &json);
void add_json_to_hospitals(const QJsonObject &json);
#endif // UTILS_H
