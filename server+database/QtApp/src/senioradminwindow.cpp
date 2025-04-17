#include "senioradminwindow.h"
#include "ui_senioradminwindow.h"
#include <QString>
#include "utils.h"
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <nlohmann/json.hpp>
#include "../client/include/client_senior_admin.hpp"
#include "../client/include/common_for_all.hpp"

SeniorAdminWindow::SeniorAdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SeniorAdminWindow)
{
    ui->setupUi(this);
}

SeniorAdminWindow::~SeniorAdminWindow()
{
    delete ui;
}

void SeniorAdminWindow::on_add_junior_administrator_button_clicked()
{
    ui->error_add_junior->setText("");
    QString last_name=ui->last_name_form->text();
    QString first_name=ui->first_name_form->text();
    QString middle_name=ui->middle_name_form->text();
    QString phone_number=ui->phone_number_form->text();
    if(!is_latin_or_dash(last_name)){
        ui->error_add_junior->setText("Incorrect format for the last name.");
        return;
    }
    if(!is_latin_or_dash(first_name)){
        ui->error_add_junior->setText("Incorrect format for the first name.");
        return;
    }
    if(!is_latin_or_dash(middle_name)){
        ui->error_add_junior->setText("Incorrect format for the middle name. If there is no middle name, enter '-'.");
        return;
    }
    if(!is_validated_phone(phone_number)){
        ui->error_add_junior->setText("Incorrect format for the phone number.");
        return;
    }
    QJsonObject junior_admin_data = {
        {"last_name", last_name},
        {"first_name", first_name},
        {"patronymic", middle_name},
        {"phone", phone_number},
        {"user_type", "junior admin"}
    };
    // Преобразование QJsonObject в строку, если функция add_junior_admin принимает std::string
    QByteArray jsonData = QJsonDocument(junior_admin_data).toJson();
    std::string jsonString = jsonData.toStdString();
    nlohmann::json j = nlohmann::json::parse(jsonString);

    senior_admin::senior_admin_client client(1);
    client.add_junior_admin(j);
}


void SeniorAdminWindow::on_add_new_hospital_button_clicked()
{
    ui->error_add_hospital->setText("");
    QString region=ui->region_form->text();
    QString settlement_type= ui->settlement_type_form->text();
    QString settlement_name=ui->settlement_name_form->text();
    QString street = ui->street_form->text();
    QString house = ui->house_form->text();
    QString hospital_full_name =ui->hospital_full_name_form->text();
    QString id_of_junior_administrator =ui->id_of_junior_administrator_form->text();
    if(!is_latin_or_dash(region)){
        ui->error_add_hospital->setText("Incorrect format for the region.");
        return;
    }
    if(!is_latin_or_dash(settlement_type)){
        ui->error_add_hospital->setText("Incorrect format for the settlement type.");
        return;
    }
    if(!is_latin_or_dash(settlement_name)){
        ui->error_add_hospital->setText("Incorrect format for the settlement name.");
        return;
    }
    if(!is_latin_or_dash(street)){
        ui->error_add_hospital->setText("Incorrect format for the street.");
        return;
    }
    if(!is_number(house)){
        ui->error_add_hospital->setText("Incorrect format for the house.");
        return;
    }
    if(!is_number(id_of_junior_administrator)){
        ui->error_add_hospital->setText("Incorrect format for id of junior administrator.");
        return;
    }
    QJsonObject hospital_data = {
        {"region", region},
        {"settlement_type", settlement_type},
        {"settlement_name", settlement_name},
        {"street", street},
        {"house", house},
        {"full_name", hospital_full_name},
        {"administrator_id", id_of_junior_administrator}};
    QByteArray jsonData = QJsonDocument(hospital_data).toJson();
    std::string jsonString = jsonData.toStdString();
    nlohmann::json j = nlohmann::json::parse(jsonString);
    senior_admin::senior_admin_client client(1);
    client.add_hospital(j);
}


void SeniorAdminWindow::on_get_users_table_button_clicked()
{
    senior_admin::senior_admin_client client(1);
    // Получаем данные в формате nlohmann::json
    nlohmann::json jsonData = client.get_users_table();

    // Преобразуем в строку
    std::string jsonString = jsonData.dump();

    // Создаем QJsonDocument из строки
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка при разборе JSON!";
        return;
    }

    // Получаем корневой объект JSON
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("users") || !rootObject["users"].isArray()) {
        qDebug() << "Ключ 'users' отсутствует или не является массивом!";
        return;
    }
      QJsonArray usersArray = rootObject["users"].toArray();
    QWidget *contentWidget = new QWidget(ui->users_table_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    // Добавляем данные в виджет
    for (const QJsonValue &userValue : usersArray) {
        if (userValue.isObject()) {
            QJsonObject userObj = userValue.toObject();
            QString id=userObj["id"].toString();

            QString fullName = userObj["last_name"].toString() + ", " +
                               userObj["first_name"].toString() + ", " +
                               userObj["patronymic"].toString();

            QString phoneNumber = userObj["phone"].toString();

            QString type= userObj["user_type"].toString();

            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(id+", "+fullName +", "+ phoneNumber+", "+type);
            label->setStyleSheet("border: 1px solid black; padding: 5px;");

            // Добавляем QLabel в layout
            layout->addWidget(label);
        }
    }
    // Устанавливаем layout для contentWidget
    contentWidget->setLayout(layout);
    // Устанавливаем contentWidget в QScrollArea
    ui->users_table_scroll->setWidget(contentWidget);
}


void SeniorAdminWindow::on_get_hospitals_table_button_clicked()
{
    senior_admin::senior_admin_client client(1);
    // Получаем данные в формате nlohmann::json
    nlohmann::json jsonData = client.get_hospitals_table();

    // Преобразуем в строку
    std::string jsonString = jsonData.dump();

    // Создаем QJsonDocument из строки
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка при разборе JSON!";
        return;
    }
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("hospitals") || !rootObject["hospitals"].isArray()) {
        qDebug() << "Ключ 'hospitals' отсутствует или не является массивом!";
        return;
    }
    QJsonArray hospitalsArray = rootObject["hospitals"].toArray();
    QWidget *contentWidget = new QWidget(ui->hospitals_table_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    for (const QJsonValue &hospitalValue : hospitalsArray) {
        if (hospitalValue.isObject()) {
            QJsonObject hospitalObj = hospitalValue.toObject();
            QString text =hospitalObj["hospital_id"].toString()+", "+hospitalObj["region"].toString()+", "+hospitalObj["settlement_type"].toString()+", "+hospitalObj["settlement_name"].toString()+", "+hospitalObj["street"].toString()+", "+hospitalObj["house"].toString()+", "+hospitalObj["full_name"].toString()+", "+hospitalObj["administrator_id"].toString();
            QLabel *label = new QLabel(contentWidget);
            label->setText(text);
            label->setStyleSheet("border: 1px solid black; padding: 5px;");
            // Добавляем QLabel в layout
            layout->addWidget(label);
        }
    }
    // Устанавливаем layout для contentWidget
    contentWidget->setLayout(layout);
    // Устанавливаем contentWidget в QScrollArea
    ui->hospitals_table_scroll->setWidget(contentWidget);
}

