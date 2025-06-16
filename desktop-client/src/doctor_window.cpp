#include "doctor_window.h"
#include <QDebug>  // Added for qDebug()
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>  // Added for QLabel
#include <QString>
#include <QVBoxLayout>  // Added for QVBoxLayout
#include <iostream>
#include <utils.h>
#include <nlohmann/json.hpp>
#include "client_doctor.hpp"
#include "ui_doctor_window.h"

DoctorWindow::DoctorWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DoctorWindow) {
    ui->setupUi(this);
}

DoctorWindow::~DoctorWindow() {
    delete ui;
}

void DoctorWindow::on_view_schedule_button_clicked() {
    doctor::doctor_client client(get_user_id());
    std::cerr<<get_user_id();
    nlohmann::json schedule = client.get_schedule();
    std::string jsonString = schedule.dump();
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("schedule") || !rootObject["schedule"].isArray()) {
        qDebug() << "Ключ 'schedule' отсутствует или не является массивом!";
        return;
    }
    QJsonArray scheduleArray = rootObject["schedule"].toArray();
    QWidget *contentWidget = new QWidget(ui->schedule_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    for (const QJsonValue &entry : scheduleArray) {
        if (entry.isObject()) {
            QJsonObject userObj = entry.toObject();
            QString date = userObj["appointment_date"].toString();
            QString time = userObj["appointment_time"].toString();
            QString cabinet = userObj["cabinet_number"].toString();
            QString hospital_name = userObj["full_name"].toString();
            QString first_name = userObj["first_name"].toString();
            QString last_name = userObj["last_name"].toString();
            QString patronymic = userObj["patronymic"].toString();
            QString phone = userObj["phone"].toString();
            first_name = (first_name == "") ? "-" : first_name;
            last_name = (last_name == "") ? "-" : last_name;
            patronymic = (patronymic == "") ? "-" : patronymic;
            phone = (phone == "") ? "-" : phone;
            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(
                date + ", " + time + ", " + cabinet + ", " + hospital_name +
                ", " + first_name + ", " + last_name + ", " + patronymic +
                ", " + phone
            );
            label->setStyleSheet("border: 1px solid black; padding: 5px;");

            // Добавляем QLabel в layout
            layout->addWidget(label);
        }
    }
    // Устанавливаем layout для contentWidget
    contentWidget->setLayout(layout);
    // Устанавливаем contentWidget в QScrollArea
    ui->schedule_scroll->setWidget(contentWidget);
}

void DoctorWindow::set_user_id(int id) {
    user_id = id;
}

int DoctorWindow::get_user_id() {
    return user_id;
}
void DoctorWindow::on_pushButton_clicked()
{
    ui->edit_error_label->setText("");
    QString first_name = ui->edit_first_name->text();
    QString last_name = ui->edit_last_name->text();
    QString middle_name = ui->edit_middle_name->text();
    QString new_password = ui->edit_new_password->text();
    QString new_password_again = ui->edit_new_password_again->text();
    QString current_password = ui->current_password->text();
    QString phone = ui->edit_phone->text();
    if (phone != "" && !is_validated_phone(phone)) {
        ui->edit_error_label->setText("Incorrect formar for phone number");
        return;
    }
    if (first_name != "" && !is_latin_or_dash(first_name)) {
        ui->edit_error_label->setText("Incorrect formar for first name");
        return;
    }
    if (last_name != "" && !is_latin_or_dash(last_name)) {
        ui->edit_error_label->setText("Incorrect formar for last name");
        return;
    }
    if (middle_name != "" && !is_latin_or_dash(middle_name)) {
        ui->edit_error_label->setText("Incorrect formar for middle name");
        return;
    }
    if (new_password != "" && !is_valid_password(new_password)) {
        ui->edit_error_label->setText("Incorrect formar for new password");
        return;
    }
    if (new_password != "" && new_password_again == "") {
        ui->edit_error_label->setText("Enter your new password again");
        return;
    }
    if (new_password != "" && new_password != new_password_again) {
        ui->edit_error_label->setText("New passwords do not match");
        return;
    }
    if (current_password == "") {
        ui->edit_error_label->setText("Enter your current password");
        return;
    }
    if (!is_valid_password(current_password)) {
        ui->edit_error_label->setText("Incorrect format for current password");
        return;
    }
    QJsonObject json;
    json["user_id"] = get_user_id();
    json["current_password"] = current_password;
    if (middle_name != "") {
        json["patronymic"] = middle_name;
    }
    if (first_name != "") {
        json["first_name"] = first_name;
    }
    if (last_name != "") {
        json["last_name"] = last_name;
    }
    if (phone != "") {
        json["phone"] = phone;
    }
    if (new_password != "") {
        json["new_password"] = new_password;
        json["new_password_repeat"] = new_password_again;
    }
    QJsonDocument doc(json);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    nlohmann::json edit_data = nlohmann::json::parse(jsonString.toStdString());
    doctor::doctor_client client(get_user_id());
    nlohmann::json response = client.edit_doctor_profile(edit_data);
    if (response.contains("error")) {
        ui->edit_error_label->setText(QString::fromStdString(response["error"])
                                      );
    } else if (response.contains("message")) {
        ui->edit_error_label->setText(QString::fromStdString(response["message"]
                                                             ));
    }
    std::cerr<<get_user_id();
}

