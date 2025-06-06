#include "homepage.h"
#include <curl/curl.h>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QString>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "client_patient.hpp"
#include "common_for_all.hpp"
#include "appointment.h"
#include "ui_homepage.h"
int patient_id = 0;

homepage::homepage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::homepage) {
    ui->setupUi(this);
    on_appointments_button_clicked();
    patient_id = get_user_id();
}

void homepage::set_user_id(int id) {
    user_id = id;
}

int homepage::get_user_id() {
    return user_id;
}

homepage::~homepage() {
    delete ui;
}

void homepage::make_all_basic() {
    const QString baseStyle =
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(64, 64, 80, 100);"
        "   border-radius: 10px;"
        "}";

    // Apply to all buttons
    ui->appointments_button->setStyleSheet(baseStyle);
    ui->doctors_button->setStyleSheet(baseStyle);
    ui->profile_button->setStyleSheet(baseStyle);
    ui->hospitals_button->setStyleSheet(baseStyle);
    ui->settings_button->setStyleSheet(baseStyle);
    ui->notifications_button->setStyleSheet(baseStyle);
}

void homepage::on_appointments_button_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->appointments_page);
    make_all_basic();
    ui->appointments_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: rgb(64, 64, 80);"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(64, 64, 100);"
        "   border-radius: 10px;"
        "}"
    );
    patient::patient_client client(get_user_id());
    nlohmann::json appointments_json =
        client.patient_appointments(get_user_id());

    QWidget *container = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Обработка записей (как в вашем коде)
    auto appointments_array = appointments_json["appointments"];
    if (appointments_array.empty()) {
        QLabel *noAppointmentsLabel =
            new QLabel("You don't have any appointments");
        noAppointmentsLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(noAppointmentsLabel);
    } else {
        for (const auto &appt : appointments_array) {
            QWidget *apptWidget = new QWidget();
            apptWidget->setStyleSheet(
                "border: 1px solid #ddd; border-radius: 5px; padding: 10px;"
                "background-color: #f9f9f9; margin-bottom: 10px;"
            );

            QVBoxLayout *apptLayout = new QVBoxLayout(apptWidget);
            apptLayout->setSpacing(5);

            // Форматируем дату и время
            QDate date = QDate::fromString(
                QString::fromStdString(appt["appointment_date"]), "yyyy-MM-dd"
            );
            QTime time = QTime::fromString(
                QString::fromStdString(appt["appointment_time"]), "HH:mm:ss"
            );

            // Основная информация
            QLabel *header =
                new QLabel(QString("<b>%1 %2</b> from %3 ₽.")
                               .arg(date.toString("dd.MM.yyyy"))
                               .arg(time.toString("HH:mm"))
                               .arg(QString::fromStdString(appt["price"])));
            header->setStyleSheet("font-size: 14px;");

            QLabel *doctorInfo =
                new QLabel(QString("<b>%1</b> (%2)")
                               .arg(QString::fromStdString(appt["doctor_name"]))
                               .arg(QString::fromStdString(appt["specialty"])));
            doctorInfo->setStyleSheet("font-size: 13px;");

            QLabel *hospitalInfo = new QLabel(
                QString("%1, %2 %3, %4")
                    .arg(QString::fromStdString(appt["hospital_name"]))
                    .arg(QString::fromStdString(appt["street"]))
                    .arg(QString::fromStdString(appt["house"]))
                    .arg(QString::fromStdString(appt["settlement_name"]))
            );
            hospitalInfo->setStyleSheet("font-size: 12px; color: #555;");

            QLabel *contactLabel =
                new QLabel(QString("Phone number: %1")
                               .arg(QString::fromStdString(appt["admin_phone"]))
                );
            contactLabel->setStyleSheet("font-size: 12px;");
            apptLayout->addWidget(header);
            apptLayout->addWidget(doctorInfo);
            apptLayout->addWidget(hospitalInfo);
            apptLayout->addWidget(contactLabel);
            mainLayout->addWidget(apptWidget);
        }
    }

    // Устанавливаем новый виджет в scroll area
    ui->appointments_scroll->setWidget(container);
    ui->appointments_scroll->setWidgetResizable(true);
}

void homepage::on_profile_button_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->profile_page);
    make_all_basic();
    ui->profile_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: rgb(64, 64, 80);"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(64, 64, 100);"
        "   border-radius: 10px;"
        "}"
    );
    ui->label_5->setText("Your id: " + QString::number(get_user_id()));
}

void homepage::on_doctors_button_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->doctors_page);
    make_all_basic();
    ui->doctors_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: rgb(64, 64, 80);"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(64, 64, 100);"
        "   border-radius: 10px;"
        "}"
    );
    patient::patient_client client(get_user_id());
    nlohmann::json regions = client.get_regions();
    auto regions_array = regions["regions"].get<std::vector<std::string>>();
    ui->combo_box_region->clear();
    for (const auto &name : regions_array) {
        ui->combo_box_region->addItem(QString::fromStdString(name));
    }
    nlohmann::json types = client.get_settlement_types();
    auto settlement_types_array =
        types["settlement_types"].get<std::vector<std::string>>();
    ui->combo_box_settlement_type->clear();
    for (const auto &name : settlement_types_array) {
        ui->combo_box_settlement_type->addItem(QString::fromStdString(name));
    }
    nlohmann::json settlement_names = client.get_settlement_names();
    auto settlement_names_array =
        settlement_names["settlement_names"].get<std::vector<std::string>>();
    ui->combo_box_settlement_name->clear();
    for (const auto &name : settlement_names_array) {
        ui->combo_box_settlement_name->addItem(QString::fromStdString(name));
    }
    nlohmann::json specialties = client.get_specialties();
    auto specialties_array =
        specialties["specialties"].get<std::vector<std::string>>();
    ui->combo_box_specialty->clear();
    for (const auto &spec : specialties_array) {
        ui->combo_box_specialty->addItem(QString::fromStdString(spec));
    }
    nlohmann::json hospital_names = client.get_hospital_full_names();
    auto hospital_names_array =
        hospital_names["hospital_full_names"].get<std::vector<std::string>>();
    ui->combo_box_hospital->clear();
    for (const auto &spec : hospital_names_array) {
        ui->combo_box_hospital->addItem(QString::fromStdString(spec));
    }
    on_apply_filtres_button_clicked();
}

void homepage::on_hospitals_button_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->hospitals_page);
    make_all_basic();
    ui->hospitals_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: rgb(64, 64, 80);"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(64, 64, 100);"
        "   border-radius: 10px;"
        "}"
    );
}

void homepage::on_notifications_button_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->notifications_page);
    make_all_basic();
    ui->notifications_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: rgb(64, 64, 80);"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(64, 64, 100);"
        "   border-radius: 10px;"
        "}"
    );
}

void homepage::on_settings_button_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->settings_page);
    make_all_basic();
    ui->settings_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   font: 15pt 'Arial';"
        "   text-align: left;"
        "   padding-left: 10%;"
        "   background-color: rgb(64, 64, 80);"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(64, 64, 100);"
        "   border-radius: 10px;"
        "}"
    );
}

void homepage::on_apply_filtres_button_clicked() {
    QString region = ui->combo_box_region->currentText();
    QString settlement_type = ui->combo_box_settlement_type->currentText();
    QString settlement_name = ui->combo_box_settlement_name->currentText();
    QString specialty = ui->combo_box_specialty->currentText();
    QJsonObject json;
    json["region"] = region;
    json["settlement_type"] = settlement_type;
    json["settlement_name"] = settlement_name;
    json["specialty"] = specialty;
    QJsonDocument doc(json);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    nlohmann::json filtre_data =
        nlohmann::json::parse(jsonString.toStdString());
    patient::patient_client client(get_user_id());
    nlohmann::json response = client.search_doctors(filtre_data);
    const auto &doctors = response["doctors"];
    std::vector<Doctor> doctor_array;
    for (const auto &doctor_json : doctors) {
        Doctor doc;
        doc.doctor_id = doctor_json["doctor_id"].get<int>();
        doc.rating = doctor_json["average_rate"].get<double>();
        doc.experience = doctor_json["experience"].get<int>();
        doc.name =
            QString::fromStdString(doctor_json["fio"].get<std::string>());
        doc.price = doctor_json["price"].get<int>();
        doc.specialty =
            QString::fromStdString(doctor_json["specialty"].get<std::string>());
        doctor_array.push_back(doc);
    }
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setAlignment(Qt::AlignTop);

    // Добавляем карточки
    for (const Doctor &doctor : doctor_array) {
        create_doctor_card(doctor, scrollLayout);
    }

    // Настраиваем ScrollArea
    ui->doctor_schedule->setWidget(scrollContent);
    ui->doctor_schedule->setWidgetResizable(true);

    // Устанавливаем отступы
    scrollContent->setStyleSheet("padding: 5px;");
}

void homepage::create_doctor_card(const Doctor &doctor, QVBoxLayout *layout) {
    // Создаем контейнер для карточки
    QWidget *card = new QWidget();
    card->setStyleSheet(
        "border: 1px solid #ddd; border-radius: 8px; padding: 15px; margin: "
        "2px; background: white;"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    // ФИО врача
    QLabel *nameLabel = new QLabel(doctor.name);
    nameLabel->setStyleSheet("font-size: 16px; color: #333;");
    cardLayout->addWidget(nameLabel);

    // Основная информация
    cardLayout->addWidget(new QLabel("Specialty: " + doctor.specialty));

    // Доп. информация в строку
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(new QLabel(
        "Experience: " + QString::number(doctor.experience) + " years"
    ));
    infoLayout->addWidget(new QLabel(
        "Rating: " + (QString::number(doctor.rating, 'f', 1) == "0.0"
                          ? "-"
                          : QString::number(doctor.rating, 'f', 1))
    ));
    infoLayout->addWidget(
        new QLabel("Price from " + QString::number(doctor.price) + " ₽")
    );
    cardLayout->addLayout(infoLayout);

    // Кнопка записи
    QPushButton *appointmentBtn = new QPushButton("Make an appointment");
    appointmentBtn->setStyleSheet(
        "QPushButton {background-color:rgb(87, 203, 95); color: white; border: "
        "none; padding: 8px; border-radius: 4px;}"
        "QPushButton:hover {"
        "   background-color: rgb(124, 239, 132);"
        "}"
    );
    patient_id = get_user_id();
    QAbstractButton::connect(appointmentBtn, &QPushButton::clicked, [&]() {
        Appointment *appointmentWindow = new Appointment();  // Создаем окно
        appointmentWindow->show();
        appointmentWindow->doctor_id = doctor.doctor_id;
        appointmentWindow->user_id = patient_id;
    });

    cardLayout->addWidget(appointmentBtn);
    layout->addWidget(card);
}
