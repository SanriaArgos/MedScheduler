#include "homepage.h"
#include <curl/curl.h>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTime>
#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "appointment.h"
#include "client_patient.hpp"
#include "common_for_all.hpp"
#include "ui_homepage.h"
#include "utils.h"
int patient_id = 0;

homepage::homepage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::homepage) {
    ui->setupUi(this);
    patient_id = get_user_id();
    ui->profile_parametrs_box->hide();
    ui->appointments_container = new QWidget();
    auto *layout = new QVBoxLayout(ui->appointments_container);
    ui->appointments_container->setLayout(layout);
    ui->appointments_scroll->setWidget(ui->appointments_container);
    ui->appointments_scroll->setWidgetResizable(true);
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
    patient::patient_client client(get_user_id());
    nlohmann::json appointments_json =
        client.get_patient_appointments(get_user_id());

    // сохраняем в векторе
    auto appointments_array = appointments_json["appointments"];
    all_records.clear();
    for (const auto &appt : appointments_array) {
        Record record;
        record.admin_phone =
            QString::fromStdString(appt["admin_phone"].get<std::string>());
        record.appointment_date = QDate::fromString(
            QString::fromStdString(appt["appointment_date"].get<std::string>()),
            "yyyy-MM-dd"
        );
        record.appointment_time = QTime::fromString(
            QString::fromStdString(appt["appointment_time"].get<std::string>()),
            "hh:mm:ss"
        );
        record.doctor_name =
            QString::fromStdString(appt["doctor_name"].get<std::string>());
        record.hospital_name =
            QString::fromStdString(appt["hospital_name"].get<std::string>());
        record.house = QString::fromStdString(appt["house"].get<std::string>());
        record.price = QString::fromStdString(appt["price"].get<std::string>());
        record.region =
            QString::fromStdString(appt["region"].get<std::string>());
        record.settlement_name =
            QString::fromStdString(appt["settlement_name"].get<std::string>());
        record.settlement_type =
            QString::fromStdString(appt["settlement_type"].get<std::string>());
        record.specialty =
            QString::fromStdString(appt["specialty"].get<std::string>());
        record.street =
            QString::fromStdString(appt["street"].get<std::string>());
        // record.record_id=appt["record_id"].get<int>();
        all_records.push_back(record);
    }
    fill_appointments_scroll(all_records);
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
    if (ui->doctor_schedule->widget()) {
        delete ui->doctor_schedule->widget();
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
    int local_patient_id = patient_id;
    QAbstractButton::connect(
        appointmentBtn, &QPushButton::clicked,
        [doctor, local_patient_id]() {
            Appointment *appointmentWindow = new Appointment(doctor.doctor_id);
            appointmentWindow->show();
            // appointmentWindow->doctor_id = doctor.doctor_id;
            appointmentWindow->user_id = local_patient_id;
        }
    );

    cardLayout->addWidget(appointmentBtn);
    layout->addWidget(card);
}

void homepage::on_edit_profile_button_clicked() {
    if (ui->profile_parametrs_box->isVisible()) {
        ui->profile_parametrs_box->hide();
        ui->label_for_enter_data->setText("");
    } else {
        ui->profile_parametrs_box->show();
        ui->label_for_enter_data->setText("Enter data you want to change");
    }
}

void homepage::on_apply_changes_edit_clicked() {
    ui->edit_error_label->setText("");
    QString first_name = ui->first_name_line_edit->text();
    QString last_name = ui->last_name_line_edit->text();
    QString middle_name = ui->middle_name_line_edit->text();
    QString new_password = ui->new_password_line_edit->text();
    QString new_password_again = ui->new_password_again_edit->text();
    QString current_password = ui->confirm_changes_line_edit->text();
    QString phone = ui->phone_number_line_edit->text();
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
    patient::patient_client client(get_user_id());
    nlohmann::json response = client.edit_patient_profile(edit_data);
    if (response.contains("error")) {
        ui->edit_error_label->setText(QString::fromStdString(response["error"])
        );
    } else if (response.contains("message")) {
        ui->edit_error_label->setText(QString::fromStdString(response["message"]
        ));
    }
}

void homepage::on_delete_account_button_clicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm",
        "Are you sure you want to delete account? You cannot revert this "
        "action.",
        static_cast<QMessageBox::StandardButtons>(
            QMessageBox::No | QMessageBox::Yes
        )
    );

    if (reply == QMessageBox::Yes) {
        patient::patient_client client(get_user_id());
        nlohmann::json response = client.delete_self_account(get_user_id());
        if (response.contains("error")) {
            QString errorText = QString::fromStdString(response["error"]);
            std::cerr << "Error: " << errorText.toStdString() << std::endl;
            QMessageBox::warning(
                this, "Error", "Failed to delete account:\n" + errorText
            );
        } else {
            QMessageBox::information(
                this, "Account deleted", "Your account was deleted."
            );
            this->close();
        }
    }
}

void homepage::sort_records(std::vector<Record> &recs, bool newestFirst) {
    std::sort(
        recs.begin(), recs.end(),
        [newestFirst](const Record &a, const Record &b) {
            if (a.appointment_date != b.appointment_date) {
                if (newestFirst) {
                    return a.appointment_date > b.appointment_date;
                } else {
                    return a.appointment_date < b.appointment_date;
                }
            }
            if (newestFirst) {
                return a.appointment_time > b.appointment_time;
            } else {
                return a.appointment_time < b.appointment_time;
            }
        }
    );
}

void homepage::on_all_button_clicked() {
    // Сначала берём копию всех записей
    std::vector<Record> filtered = all_records;

    // Определяем порядок сортировки: currentIndex() == 0 — "Сначала новые"
    bool newestFirst = (ui->sortCombo->currentIndex() == 0);
    sort_records(filtered, newestFirst);

    fill_appointments_scroll(filtered);
}

void homepage::on_upcoming_button_clicked() {
    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();
    std::vector<Record> upcoming;
    for (const auto &r : all_records) {
        if (r.appointment_date > today ||
            (r.appointment_date == today && r.appointment_time >= now)) {
            upcoming.push_back(r);
        }
    }

    bool newestFirst = (ui->sortCombo->currentIndex() == 0);
    sort_records(upcoming, newestFirst);

    fill_appointments_scroll(upcoming);
}

void homepage::on_completed_button_clicked() {
    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();
    std::vector<Record> completed;
    for (const auto &r : all_records) {
        if (r.appointment_date < today ||
            (r.appointment_date == today && r.appointment_time < now)) {
            completed.push_back(r);
        }
    }

    bool newestFirst = (ui->sortCombo->currentIndex() == 0);
    sort_records(completed, newestFirst);

    fill_appointments_scroll(completed);
}

void homepage::fill_appointments_scroll(const std::vector<Record> &records) {
    // 1) Берём layout внутри appointments_container и полностью очищаем его:
    auto layout =
        qobject_cast<QVBoxLayout *>(ui->appointments_container->layout());
    if (!layout) {
        return;
    }

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        // удаляем вложенные виджеты
        if (auto w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }

    QDate today = QDate::currentDate();

    // 2) Для каждой записи создаём «карточку»
    for (const auto &r : records) {
        QWidget *card = new QWidget();
        card->setStyleSheet(
            "background-color: white;"
            "border: 1px solid #888;"
            "border-radius: 8px;"
        );

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);  // внутренние отступы
        cardLayout->setSpacing(6);

        // 1-я строка: дата и время
        QLabel *dateTime = new QLabel(
            r.appointment_date.toString("dd.MM.yyyy") + " " +
            r.appointment_time.toString("hh:mm")
        );
        dateTime->setStyleSheet(
            "font-size: 13pt; font-weight: bold; border: none;"
        );
        cardLayout->addWidget(dateTime);

        // 2-я строка: ФИО врача
        QLabel *name = new QLabel(r.doctor_name);
        name->setStyleSheet("font-size: 12.5pt; border: none;");
        cardLayout->addWidget(name);

        // 3-я строка: специальность и цена
        QLabel *specialty =
            new QLabel(r.specialty + " price from " + r.price + " rub.");
        specialty->setStyleSheet("font-size: 12pt; color: #444; border: none;");
        cardLayout->addWidget(specialty);

        // 4-я строка: адрес (посёлок, улица, дом)
        QLabel *address =
            new QLabel(r.settlement_name + ", " + r.street + ", " + r.house);
        address->setStyleSheet("font-size: 12pt; color: #555; border: none;");
        cardLayout->addWidget(address);

        // 5-я строка: кнопка на всю ширину
        QPushButton *action = new QPushButton();
        action->setMinimumHeight(36);
        action->setCursor(Qt::PointingHandCursor);
        action->setStyleSheet(
            "font-size: 12pt; color: white; border: none; border-radius: 5px; "
            "margin-top: 8px;"
        );

        if (r.appointment_date > today) {
            action->setText("Cancel appointment");
            action->setStyleSheet(
                action->styleSheet() + "background-color: #D32F2F;"
            );
        } else if (r.appointment_date < today) {
            action->setText("Leave feedback");
            action->setStyleSheet(
                action->styleSheet() + "background-color: #1976D2;"
            );
        } else {
            action->setVisible(false);
        }

        cardLayout->addWidget(action);
        layout->addWidget(card);
    }

    // 3) Опционально: «раздвинуть» карточки и не прижимать к низу
    layout->addStretch();
}
