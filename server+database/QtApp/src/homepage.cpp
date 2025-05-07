#include "homepage.h"
#include "appointment.h"
#include <curl/curl.h>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QPushButton>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "../client/include/common_for_all.hpp"
#include "ui_homepage.h"
#include <vector>
homepage::homepage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::homepage) {
    ui->setupUi(this);
    on_appointments_button_clicked();
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
    //ui->combo_box_settlement_type->addItem("тип"); таким образом позаполнять все combo_box
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


void homepage::on_combo_box_region_currentTextChanged(const QString &region_name)
{
    //nlohmann::json types = get_settlement_types();
    //циклом добавить ui->combo_box_settlement_type->addItem("тип");
}


void homepage::on_combo_box_settlement_type_currentTextChanged(const QString &settlement_type)
{
    //nlohmann::json names = get_settlement_names();
    //циклом добавить ui->combo_box_settlement_name>addItem("город");
}


void homepage::on_combo_box_settlement_name_currentTextChanged(const QString &settlement_name)
{
    //nlohmann::json specialties = get_specialties();
    //циклом добавить ui->combo_box_settlement_name>addItem("город");
}


void homepage::on_combo_box_specialty_currentTextChanged(const QString &specialty)
{

}


void homepage::on_apply_filtres_button_clicked()
{
    QString region = ui->combo_box_region->currentText();
    QString settlement_type = ui->combo_box_settlement_type->currentText();
    QString settlement_name = ui->combo_box_settlement_name->currentText();
    QString specialty = ui->combo_box_specialty->currentText();
    QJsonObject json;
    json["region"] = region;
    json["settlement_type"] = settlement_type;
    json["settlemnet_name"] = settlement_name;
    json["specialty"] = specialty;
    QJsonDocument doc(json);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    nlohmann::json filtre_data = nlohmann::json::parse(jsonString.toStdString());
    //тут надо отправить запрос и получить джейсон
    QJsonArray array;
    std::vector<Doctor> doctor_array;
    doctor_array = {
        {"Иванов", "Алексей", "Петрович", "Кардиолог", "ГКБ №1", "МГМУ", 2500, 101, 15, 4.8},
        {"Петрова", "Мария", "Сергеевна", "Невролог", "Поликлиника №3", "РНИМУ", 1800, 102, 8, 4.6}
    };
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


void homepage::on_combo_box_hospital_currentTextChanged(const QString &hospital)
{

}

void create_doctor_card(const Doctor &doctor, QVBoxLayout *layout) {
    // Создаем контейнер для карточки
    QWidget *card = new QWidget();
    card->setStyleSheet("border: 1px solid #ddd; border-radius: 8px; padding: 15px; margin: 2px; background: white;");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    // ФИО врача
    QLabel *nameLabel = new QLabel(
        QString("<b>%1 %2 %3</b>").arg(doctor.last_name, doctor.first_name, doctor.patronym)
        );
    nameLabel->setStyleSheet("font-size: 16px; color: #333;");
    cardLayout->addWidget(nameLabel);

    // Основная информация
    cardLayout->addWidget(new QLabel("Specialty: " + doctor.specialty));
    cardLayout->addWidget(new QLabel("Hospital: " + doctor.hospital_name));

    // Доп. информация в строку
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(new QLabel("Experience: " + QString::number(doctor.experience) + " years"));
    infoLayout->addWidget(new QLabel("Rating: " + QString::number(doctor.rating, 'f', 1)));
    infoLayout->addWidget(new QLabel("Price from " + QString::number(doctor.price) + " ₽"));
    cardLayout->addLayout(infoLayout);

    // Кнопка записи
    QPushButton *appointmentBtn = new QPushButton("Make an appointment");
    appointmentBtn->setStyleSheet(
        "QPushButton {background-color:rgb(87, 203, 95); color: white; border: none; padding: 8px; border-radius: 4px;}"
        "QPushButton:hover {"
        "   background-color: rgb(124, 239, 132);"
        "}"
        );
    QAbstractButton::connect(appointmentBtn, &QPushButton::clicked, [doctor]() {
        Appointment *appointmentWindow = new Appointment(); // Создаем окно
        appointmentWindow->show();
        appointmentWindow->doctor_id=doctor.doctor_id;
    });

    cardLayout->addWidget(appointmentBtn);
    layout->addWidget(card);
}

