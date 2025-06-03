#include "doctor_window.h"
#include <QDebug>  // Added for qDebug()
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>  // Added for QLabel
#include <QString>
#include <QVBoxLayout>  // Added for QVBoxLayout
#include <iostream>
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
    nlohmann::json schedule = client.get_schedule();
    std::string jsonString = schedule.dump();
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    QJsonObject rootObject = jsonDoc.object();
    qDebug() << "Full JSON:" << jsonDoc.toJson(QJsonDocument::Indented);
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