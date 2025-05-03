#include "doctor_window.h"
#include "ui_doctor_window.h"
#include "../client/src/client_doctor.cpp"
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <nlohmann/json.hpp>

DoctorWindow::DoctorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DoctorWindow)
{
    ui->setupUi(this);
}

DoctorWindow::~DoctorWindow()
{
    delete ui;
}

void DoctorWindow::on_view_schedule_button_clicked()
{
    std::cerr<<"Your id "<< get_user_id();
    doctor::doctor_client client(get_user_id());
    nlohmann::json schedule = client.get_schedule();

    // junior_admin::junior_admin_client client(get_user_id());
    // nlohmann::json full_response  = client.get_doctor_schedule(doctor_id.toInt());
    std::string jsonString = schedule.dump();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    QJsonObject rootObject = jsonDoc.object();
    qDebug() << "Full JSON:" << jsonDoc.toJson(QJsonDocument::Indented);
}

void DoctorWindow::set_user_id(int id){
    user_id=id;
}
int DoctorWindow::get_user_id(){
    return user_id;
}