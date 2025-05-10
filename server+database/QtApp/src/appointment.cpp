#include "appointment.h"
#include "ui_appointment.h"
#include <QDate>
#include <QVector>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include "../client/include/client_patient.hpp"
QDate today = QDate::currentDate();
Appointment::Appointment(QWidget *parent)
    : QWidget(parent), ui(new Ui::Appointment) {
    ui->setupUi(this);
    on_date0_clicked();
    ui->date0->setText(today.addDays(0).toString("dd.MM"));
    ui->date1->setText(today.addDays(1).toString("dd.MM"));
    ui->date2->setText(today.addDays(2).toString("dd.MM"));
    ui->date3->setText(today.addDays(3).toString("dd.MM"));
    ui->date4->setText(today.addDays(4).toString("dd.MM"));
    ui->date5->setText(today.addDays(5).toString("dd.MM"));
    ui->date6->setText(today.addDays(6).toString("dd.MM"));

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

void Appointment::on_date0_clicked()
{
    make_all_basic();
    ui->date0->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(0);
    QVector<slot> slot_vector = {
        {QDate::currentDate(), QTime(9, 0), "Городская больница №1", "ул. Ленина, 10", 205},
        {QDate::currentDate().addDays(1), QTime(10, 30), "Поликлиника №3", "пр. Мира, 25", 312},
        {QDate::currentDate().addDays(1), QTime(14, 15), "Диагностический центр", "ул. Зеленая, 5", 104}
    };
    if (ui->appointments_scroll->widget()) {
        delete ui->appointments_scroll->widget();
    }

    // Создаем контейнер для слотов
    QWidget *container = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(10); // Отступ между карточками

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
        QLabel *datetimeLabel = new QLabel(
            QString("<b>%1 %2</b>")
                .arg(s.date.toString("dd.MM.yyyy"))
                .arg(s.time.toString("hh:mm"))
            );
        datetimeLabel->setStyleSheet("font-size: 14px;");

        // Добавляем элементы
        slotLayout->addWidget(datetimeLabel);
        slotLayout->addWidget(new QLabel(s.hospital));
        slotLayout->addWidget(new QLabel(s.adress));
        slotLayout->addWidget(new QLabel(QString("Cabinet: %1").arg(s.cabinet)));

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
            );

        // Подключаем кнопку (с захватом текущего слота)
        connect(bookBtn, &QPushButton::clicked, [s]() {
            QMessageBox::information(nullptr, "Запись",
                                     QString("Вы записаны на %1 в %2")
                                         .arg(s.date.toString("dd.MM.yyyy"))
                                         .arg(s.hospital));
        });

        slotLayout->addWidget(bookBtn);
        mainLayout->addWidget(slotWidget);
    }

    // Настраиваем ScrollArea
    ui->appointments_scroll->setWidget(container);
    ui->appointments_scroll->setWidgetResizable(true);
}


void Appointment::on_date1_clicked()
{
    make_all_basic();
    ui->date1->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(1);
}


void Appointment::on_date2_clicked()
{
    make_all_basic();
    ui->date2->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(2);
}


void Appointment::on_date3_clicked()
{
    make_all_basic();
    ui->date3->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(3);
}


void Appointment::on_date4_clicked()
{
    make_all_basic();
    ui->date4->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(4);
}


void Appointment::on_date5_clicked()
{
    make_all_basic();
    ui->date5->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(5);
}


void Appointment::on_date6_clicked()
{
    make_all_basic();
    ui->date6->setStyleSheet(
        "QPushButton {"
        "   color: rgb(255, 255, 255);"
        "   background-color: rgb(64, 64, 80);"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 3px"
        "}"
        );
    QDate date = today.addDays(6);
}



