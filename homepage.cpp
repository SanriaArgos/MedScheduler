#include "homepage.h"
#include "ui_homepage.h"
#include <QString>
#include <QJsonObject>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <nlohmann/json.hpp>
#include "server+database/client/src/common_for_all.cpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
homepage::homepage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homepage)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->appointments_page);
}

homepage::~homepage()
{
    delete ui;
}

void homepage::on_appointments_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->appointments_page);
}


void homepage::on_profile_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->profile_page);
}


void homepage::on_doctors_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->doctors_page);
}


void homepage::on_hospitals_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->hospitals_page);
}


void homepage::on_notifications_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->notifications_page);
}


void homepage::on_settings_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->settings_page);
}


void homepage::on_view_schedule_button_clicked()
{
    ui->view_schedule_error_label->setText("");
    QString region=ui->region_enter->text();
    QString settlement_type=ui->settlement_type_enter->text();
    QString settlement_name=ui->settlement_name_enter->text();
    QString specialty = ui->specialty_enter->text();
    QString hospital_id = ui->hospital_id_enter->text();
    QString doctor_id = ui->doctor_id_enter->text();
    QJsonObject doctor_data={
        {"region", region},
        {"settlement_type", settlement_type},
        {"settlement_name", settlement_name},
        {"specialty", specialty},
        {"hospital_id", hospital_id},
        {"doctor_id",doctor_id}
        };
    QByteArray jsonData = QJsonDocument(doctor_data).toJson();
    std::string jsonString = jsonData.toStdString();
    nlohmann::json j = nlohmann::json::parse(jsonString);
    std::string response = send_post_request("http://localhost:8080/view_doctor_schedule_for_patient", j);
    nlohmann::json schedule = nlohmann::json::parse(response);
    QWidget* containerWidget = new QWidget();
    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);
    for (const auto &entry : schedule["schedule"]) {
        QWidget* entryWidget = new QWidget();
        QHBoxLayout* entryLayout = new QHBoxLayout(entryWidget);

        // Добавляем информацию о записи
        QLabel* dateLabel = new QLabel(QString::fromStdString("Date: " + entry["appointment_date"].get<std::string>()));
        QLabel* timeLabel = new QLabel(QString::fromStdString("Time: " + entry["appointment_time"].get<std::string>()));
        QLabel* cabinetLabel = new QLabel(QString::fromStdString("Cabinet: " + entry["cabinet_number"].get<std::string>()));

        // Добавляем элементы в layout виджета записи
        entryLayout->addWidget(dateLabel);
        entryLayout->addWidget(timeLabel);
        entryLayout->addWidget(cabinetLabel);

        // Добавляем виджет записи в контейнер
        containerLayout->addWidget(entryWidget);

        // Добавляем разделитель (опционально)
        containerLayout->addWidget(new QFrame());
        containerLayout->widget()->setObjectName("separator");
        containerLayout->widget()->setStyleSheet("QFrame { border: 1px solid gray; }");
    }

    // Устанавливаем контейнер в QScrollArea
    ui->doctor_schedule->setWidget(containerWidget);
    ui->doctor_schedule->setWidgetResizable(true);
    }


