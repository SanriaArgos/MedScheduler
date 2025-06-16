#include "senior_admin_window.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <nlohmann/json.hpp>
#include "client_senior_admin.hpp"
#include "common_for_all.hpp"
#include "ui_senior_admin_window.h"
#include "utils.h"

SeniorAdminWindow::SeniorAdminWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SeniorAdminWindow) {
    ui->setupUi(this);
    on_button_get_users_clicked();
}

void SeniorAdminWindow::set_user_id(int id) {
    user_id = id;
}

int SeniorAdminWindow::get_user_id() {
    return user_id;
}

SeniorAdminWindow::~SeniorAdminWindow() {
    delete ui;
}

void SeniorAdminWindow::on_add_junior_administrator_button_clicked() {
    ui->error_add_junior->setText("");
    QString last_name = ui->last_name_form->text();
    QString first_name = ui->first_name_form->text();
    QString middle_name = ui->middle_name_form->text();
    QString phone_number = ui->phone_number_form->text();
    if (!is_latin_or_dash(last_name)) {
        ui->error_add_junior->setText("Incorrect format for the last name.");
        return;
    }
    if (!is_latin_or_dash(first_name)) {
        ui->error_add_junior->setText("Incorrect format for the first name.");
        return;
    }
    if (!is_latin_or_dash(middle_name)) {
        ui->error_add_junior->setText(
            "Incorrect format for the middle name. If there is no middle name, "
            "enter '-'."
        );
        return;
    }
    if (!is_validated_phone(phone_number)) {
        ui->error_add_junior->setText("Incorrect format for the phone number.");
        return;
    }
    QJsonObject junior_admin_data = {
        {"last_name", last_name},
        {"first_name", first_name},
        {"patronymic", middle_name},
        {"phone", phone_number},
        {"user_type", "junior admin"}};

    QByteArray jsonData = QJsonDocument(junior_admin_data).toJson();
    std::string jsonString = jsonData.toStdString();
    nlohmann::json j = nlohmann::json::parse(jsonString);

    senior_admin::senior_admin_client client(get_user_id());
    client.add_junior_admin(j);
}

void SeniorAdminWindow::on_add_new_hospital_button_clicked() {
    ui->error_add_hospital->setText("");
    QString region = ui->region_form->text();
    QString settlement_type = ui->settlement_type_form->text();
    QString settlement_name = ui->settlement_name_form->text();
    QString street = ui->street_form->text();
    QString house = ui->house_form->text();
    QString hospital_full_name = ui->hospital_full_name_form->text();
    QString id_of_junior_administrator =
        ui->id_of_junior_administrator_form->text();
    if (!is_latin_or_dash(region)) {
        ui->error_add_hospital->setText("Incorrect format for the region.");
        return;
    }
    if (!is_latin_or_dash(settlement_type)) {
        ui->error_add_hospital->setText(
            "Incorrect format for the settlement type."
        );
        return;
    }
    if (!is_latin_or_dash(settlement_name)) {
        ui->error_add_hospital->setText(
            "Incorrect format for the settlement name."
        );
        return;
    }
    if (!is_latin_or_dash(street)) {
        ui->error_add_hospital->setText("Incorrect format for the street.");
        return;
    }
    if (!is_number(house)) {
        ui->error_add_hospital->setText("Incorrect format for the house.");
        return;
    }
    if (!is_number(id_of_junior_administrator)) {
        ui->error_add_hospital->setText(
            "Incorrect format for id of junior administrator."
        );
        return;
    }
    QJsonObject hospital_data = {
        {"region", region},
        {"settlement_type", settlement_type},
        {"settlement_name", settlement_name},
        {"street", street},
        {"house", house},
        {"full_name", hospital_full_name},
        {"admin_id", id_of_junior_administrator}};

    QByteArray jsonData = QJsonDocument(hospital_data).toJson();
    std::string jsonString = jsonData.toStdString();
    nlohmann::json j = nlohmann::json::parse(jsonString);
    senior_admin::senior_admin_client client(get_user_id());
    client.add_hospital(j);
}

void SeniorAdminWindow::on_get_users_table_button_clicked() {
    senior_admin::senior_admin_client client(get_user_id());

    nlohmann::json jsonData = client.get_users_table();

    std::string jsonString = jsonData.dump();

    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка при разборе JSON!";
        return;
    }

    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("users") || !rootObject["users"].isArray()) {
        qDebug() << "Ключ 'users' отсутствует или не является массивом!";
        return;
    }
    QJsonArray usersArray = rootObject["users"].toArray();
    QWidget *contentWidget = new QWidget(ui->users_table_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    for (const QJsonValue &userValue : usersArray) {
        if (userValue.isObject()) {
            QJsonObject userObj = userValue.toObject();
            QString id = userObj["id"].toString();

            QString fullName = userObj["last_name"].toString() + ", " +
                               userObj["first_name"].toString() + ", " +
                               userObj["patronymic"].toString();

            QString phoneNumber = userObj["phone"].toString();

            QString type = userObj["user_type"].toString();

            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(
                id + ", " + fullName + ", " + phoneNumber + ", " + type
            );
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

void SeniorAdminWindow::on_get_hospitals_table_button_clicked() {
    senior_admin::senior_admin_client client(get_user_id());
    // Получаем данные в формате nlohmann::json
    nlohmann::json jsonData = client.get_hospitals_table();

    // Преобразуем в строку
    std::string jsonString = jsonData.dump();

    // Создаем QJsonDocument из строки
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка при разборе JSON!";
        return;
    }
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("hospitals") ||
        !rootObject["hospitals"].isArray()) {
        qDebug() << "Ключ 'hospitals' отсутствует или не является массивом!";
        return;
    }
    QJsonArray hospitalsArray = rootObject["hospitals"].toArray();
    QWidget *contentWidget = new QWidget(ui->hospitals_table_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    for (const QJsonValue &hospitalValue : hospitalsArray) {
        if (hospitalValue.isObject()) {
            QJsonObject hospitalObj = hospitalValue.toObject();
            QString text = hospitalObj["hospital_id"].toString() + ", " +
                           hospitalObj["region"].toString() + ", " +
                           hospitalObj["settlement_type"].toString() + ", " +
                           hospitalObj["settlement_name"].toString() + ", " +
                           hospitalObj["street"].toString() + ", " +
                           hospitalObj["house"].toString() + ", " +
                           hospitalObj["full_name"].toString() + ", " +
                           hospitalObj["administrator_id"].toString();
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

void SeniorAdminWindow::make_all_basic() {
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

    ui->button_get_hospitals->setStyleSheet(baseStyle);
    ui->button_get_users->setStyleSheet(baseStyle);
    ui->button_add_hospital->setStyleSheet(baseStyle);
    ui->button_add_junior_admin->setStyleSheet(baseStyle);
    ui->page_edit_profile->setStyleSheet(baseStyle);
}

void SeniorAdminWindow::on_button_get_hospitals_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_get_hospitals);
    make_all_basic();
    ui->button_get_hospitals->setStyleSheet(
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

void SeniorAdminWindow::on_button_get_users_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_get_users);
    make_all_basic();
    ui->button_get_users->setStyleSheet(
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

void SeniorAdminWindow::on_button_add_hospital_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_add_hospital);
    make_all_basic();
    ui->button_add_hospital->setStyleSheet(
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

void SeniorAdminWindow::on_button_add_junior_admin_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_add_junior_admin);
    make_all_basic();
    ui->button_add_junior_admin->setStyleSheet(
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

void SeniorAdminWindow::on_submit_button_clicked()
{
    ui->edit_error_label->setText("");
    QString first_name = ui->first_name_line_edit->text();
    QString last_name = ui->last_name_line_edit->text();
    QString middle_name = ui->middle_name_line_edit->text();
    QString new_password = ui->new_password_line_edit->text();
    QString new_password_again = ui->new_password_again_line_edit->text();
    QString current_password = ui->current_password_line_edit->text();
    QString phone = ui->phone_line_edit->text();
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
    senior_admin::senior_admin_client client(get_user_id());
    nlohmann::json response = client.edit_senior_admin_profile(edit_data);
    if (response.contains("error")) {
        ui->edit_error_label->setText(QString::fromStdString(response["error"])
                                      );
    } else if (response.contains("message")) {
        ui->edit_error_label->setText(QString::fromStdString(response["message"]
                                                             ));
    }
}


void SeniorAdminWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_edit_profile);
    make_all_basic();
    ui->page_edit_profile->setStyleSheet(
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

