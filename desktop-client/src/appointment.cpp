#include "appointment.h"
#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <string>
#include <vector>
#include "client_patient.hpp"
#include "ui_appointment.h"
QDate today = QDate::currentDate();

Appointment::Appointment(int doc_id, QWidget *parent)
    : QWidget(parent), doctor_id(doc_id), ui(new Ui::Appointment) {
    ui->setupUi(this);
    ui->date0->setText(today.addDays(0).toString("dd.MM"));
    ui->date1->setText(today.addDays(1).toString("dd.MM"));
    ui->date2->setText(today.addDays(2).toString("dd.MM"));
    ui->date3->setText(today.addDays(3).toString("dd.MM"));
    ui->date4->setText(today.addDays(4).toString("dd.MM"));
    ui->date5->setText(today.addDays(5).toString("dd.MM"));
    ui->date6->setText(today.addDays(6).toString("dd.MM"));
    make_all_basic();
    patient::patient_client client(user_id);
    nlohmann::json response= client.get_waitlist_count(doctor_id);
    int count = response.at("count").get<int>();
    QString num = QString::number(count);
    ui->label->setText("Available slots not suitable? People in waitlist: "+ num);
}

Appointment::~Appointment() {
    delete ui;
}

void Appointment::make_all_basic() {
    const QString baseStyle =
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: transparent;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(64, 64, 80, 100);"
        "   border-radius: 10px;"
        "}";
    ui->date0->setStyleSheet(baseStyle);
    ui->date1->setStyleSheet(baseStyle);
    ui->date2->setStyleSheet(baseStyle);
    ui->date3->setStyleSheet(baseStyle);
    ui->date4->setStyleSheet(baseStyle);
    ui->date5->setStyleSheet(baseStyle);
    ui->date6->setStyleSheet(baseStyle);
}

void Appointment::handle_day_clicked(QPushButton *clicked_button, int days) {
    // Сброс стилей всех кнопок
    make_all_basic();

    // Установка стиля для активной кнопки
    clicked_button->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px;"
        "}"
    );

    // Получение даты
    QDate date = today.addDays(days);

    // Загрузка и отображение слотов (как в вашем исходном коде)
    std::vector<slot> slot_vector;
    patient::patient_client client(user_id);
    nlohmann::json schedule_json =
        client.get_doctor_schedule_for_patient(doctor_id);
    auto schedule_array = schedule_json["schedule"];

    for (const auto &slot_json : schedule_array) {
        slot sl;
        sl.appointment_date = QDate::fromString(
            QString::fromStdString(
                slot_json["appointment_date"].get<std::string>()
            ),
            "yyyy-MM-dd"
        );
        if (sl.appointment_date != date) {
            continue;
        }
        sl.appointment_time = QTime::fromString(
            QString::fromStdString(slot_json["appointment_time"]), "hh:mm:ss"
        );
        sl.full_name = QString::fromStdString(slot_json["full_name"]);
        sl.house = QString::fromStdString(slot_json["house"]);
        sl.junior_admin_phone =
            QString::fromStdString(slot_json["junior_admin_phone"]);
        sl.region = QString::fromStdString(slot_json["region"]);
        sl.settlement_name =
            QString::fromStdString(slot_json["settlement_name"]);
        sl.settlement_type =
            QString::fromStdString(slot_json["settlement_type"]);
        sl.slot_status = QString::fromStdString(slot_json["slot_status"]);
        sl.street = QString::fromStdString(slot_json["street"]);
        sl.hospital_id = std::stoi(slot_json["hospital_id"].get<std::string>());
        slot_vector.push_back(sl);
    }
    if (ui->appointments_scroll->widget()) {
        delete ui->appointments_scroll->widget();
    }

    // Создаем контейнер для слотов
    QWidget *container = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(10);  // Отступ между карточками

    // Добавляем каждый слот
    for (const slot &s : slot_vector) {
        // Создаем виджет для одного слота
        QWidget *slotWidget = new QWidget();
        slotWidget->setStyleSheet(
            "border: 1px solid #ddd;"
            "border-radius: 5px;"
            "padding: 10px;"
            "background-color: #f9f9f9;"
        );

        QVBoxLayout *slotLayout = new QVBoxLayout(slotWidget);

        // Форматируем дату и время
        QLabel *datetimeLabel =
            new QLabel(QString("<b>%1 %2</b>")
                           .arg(s.appointment_date.toString("dd.MM.yyyy"))
                           .arg(s.appointment_time.toString("hh:mm")));
        datetimeLabel->setStyleSheet("font-size: 14px;");

        // Добавляем элементы
        slotLayout->addWidget(datetimeLabel);
        slotLayout->addWidget(new QLabel(s.full_name));
        slotLayout->addWidget(new QLabel(s.street + ", " + s.house));
        slotLayout->addWidget(
            new QLabel(QString("Hospital phone: %1").arg(s.junior_admin_phone))
        );
        if (s.slot_status == "close") {
            slotLayout->addWidget(new QLabel("Booked"));
        } else {
            // Кнопка записи
            QPushButton *bookBtn = new QPushButton("Make an appointment");
            bookBtn->setStyleSheet(
                "QPushButton {"
                "   background-color: #4CAF50;"
                "   color: white;"
                "   border: none;"
                "   padding: 5px;"
                "   border-radius: 3px;"
                "}"
                "QPushButton:hover {"
                "background-color: rgb(124, 239, 132);"
                "}"
            );
            // Подключаем кнопку (с захватом текущего слота)
            connect(
                bookBtn, &QPushButton::clicked,
                [this, s, &clicked_button, &days]() {
                    patient::patient_client client1(user_id);
                    nlohmann::json j;
                    j["doctor_id"] = doctor_id;
                    j["hospital_id"] = s.hospital_id;
                    j["patient_id"] = user_id;
                    j["appointment_date"] =
                        s.appointment_date.toString("yyyy-MM-dd").toStdString();
                    ;
                    j["appointment_time"] =
                        s.appointment_time.toString("HH:mm").toStdString();
                    ;
                    nlohmann::json response = client1.book_appointment(j);
                    if (response.value("success", false)) {
                        QMessageBox::information(
                            this, "Success",
                            "You booked the appointment!\n"
                            "Date: " +
                                s.appointment_date.toString("dd.MM.yyyy") +
                                "\n" +
                                "Time: " + s.appointment_time.toString("HH:mm")
                        );

                    } else {
                        QMessageBox::warning(
                            this, "Error", "Failed to book an appointment!"
                        );
                    }
                    this->close();
                }
            );
            slotLayout->addWidget(bookBtn);
        }
        mainLayout->addWidget(slotWidget);
    }

    // Настраиваем ScrollArea
    ui->appointments_scroll->setWidget(container);
    ui->appointments_scroll->setWidgetResizable(true);
}

void Appointment::on_date0_clicked() {
    handle_day_clicked(ui->date0, 0);
}

void Appointment::on_date1_clicked() {
    handle_day_clicked(ui->date1, 1);
}

void Appointment::on_date2_clicked() {
    handle_day_clicked(ui->date2, 2);
}

void Appointment::on_date3_clicked() {
    handle_day_clicked(ui->date3, 3);
}

void Appointment::on_date4_clicked() {
    handle_day_clicked(ui->date4, 4);
}

void Appointment::on_date5_clicked() {
    handle_day_clicked(ui->date5, 5);
}

void Appointment::on_date6_clicked() {
    handle_day_clicked(ui->date6, 6);
}

void Appointment::on_add_to_waitlist_button_clicked()
{
    patient::patient_client client(user_id);
    nlohmann::json j;
    j["doctor_id"] = doctor_id;
    j["patient_id"] = user_id;
    nlohmann::json response = client.add_patient_to_waitlist(j);
    if (response.contains("success") && response["success"]==true){
        QMessageBox::information(this, "Success", "You were added to waitlist.");
    }
    else{
        QMessageBox::critical(this, "Error", QString::fromStdString(response.at("error")));
    }
}

