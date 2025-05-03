#include "utils.h"
#include <string>
#include <cctype>
#include <QString>
#include <QRegularExpression>

bool is_latin_or_dash(const QString &str) {
    std::string s=str.toStdString();
    if (s.empty()) return false;
    for (char c : s) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '-'))) {
            return false;
        }
    }
    return true;
}

bool is_valid_password(const QString &str) {
    std::string s=str.toStdString();
    if (s.empty()) return false;
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

bool is_number(const QString &str){
    std::string s=str.toStdString();
    if (s.empty()) return false;
    for (char c : s) {
        if (!(c >= '0' && c <= '9')) {
            return false;
        }
    }
    return true;
}

bool is_valid_date(const QString &str){
    QRegularExpression regex("^\\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");
    if (regex.match(str).hasMatch()) {
        return true;
    }
    return false;
}
bool is_valid_time(const QString &str){
    QRegularExpression regex("^([01]?[0-9]|2[0-3]):[0-5][0-9]$");
    if (regex.match(str).hasMatch()) {
        return true;
    }
    return false;
}
void add_json_to_users(const QJsonObject &json){
    QString filePath = "users.json";

    QFile file(filePath);

    if (!file.exists())
    {
        qDebug() << "File does not exist.";
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Opening file for read error!";
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull())
    {
        qDebug() << "Json decomposing error!";
        return;
    }

    QJsonObject rootObject = jsonDoc.object();

    if (!rootObject.contains("users"))
    {
        qDebug() << "Key 'users' does not exist!";
        return;
    }
    if(!rootObject["users"].isArray()){
        qDebug() << "users is not array";
        return;
    }

    QJsonArray usersArray = rootObject["users"].toArray();

    usersArray.append(json);
    rootObject["users"] = usersArray;

    jsonDoc.setObject(rootObject);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file for write error!";
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
    qDebug() << "New user is succesfully added!";
}
void add_json_to_hospitals(const QJsonObject &json){
    QString filePath = "hospitals.json";

    QFile file(filePath);

    if (!file.exists())
    {
        qDebug() << "File does not exist.";
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Opening file for read error!";
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull())
    {
        qDebug() << "Json decomposing error!";
        return;
    }

    QJsonObject rootObject = jsonDoc.object();

    if (!rootObject.contains("hospitals"))
    {
        qDebug() << "Key 'hospitals' does not exist!";
        return;
    }
    if(!rootObject["hospitals"].isArray()){
        qDebug() << "hospitals is not array";
        return;
    }

    QJsonArray hospitalsArray = rootObject["hospitals"].toArray();

    hospitalsArray.append(json);
    rootObject["hospitals"] = hospitalsArray;

    jsonDoc.setObject(rootObject);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Opening file for write error!";
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
    qDebug() << "New hospital is succesfully added!";
}
