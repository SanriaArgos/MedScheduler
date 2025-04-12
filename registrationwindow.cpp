#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QString>
#include <utils.h>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include "client_auth.hpp"
#include "client_doctor.hpp"
#include "client_junior_admin.hpp"
#include "client_senior_admin.hpp"
#include "client_patient.hpp"
RegistrationWindow::RegistrationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

void RegistrationWindow::on_pushButton_clicked()
{
    ui->invalid_text->setText("");
    QString last_name = ui->last_name_form->text();
    QString first_name = ui->first_name_form->text();
    QString middle_name = ui->middle_name_form->text();
    QString phone_number = ui->phone_number_form->text();
    QString password = ui->password_form->text();
    QString password_again = ui->password_again_form->text();
    if(!is_latin_or_dash(last_name)){
        ui->invalid_text->setText("Incorrect format for the last name.");
        return;
    }
    if(!is_latin_or_dash(first_name)){
        ui->invalid_text->setText("Incorrect format for the first name.");
        return;
    }
    if(!is_latin_or_dash(middle_name)){
        ui->invalid_text->setText("Incorrect format for the middle name. If there is no middle name, enter '-'.");
        return;
    }
    if(!is_validated_phone(phone_number)){
        ui->invalid_text->setText("Incorrect format for the phone number.");
        return;
    }
    if (password.size() < 8) {
        ui->invalid_text->setText("Must be at least 8 symbols in the password!");
        return;
    }
    if(!is_valid_password(password)){
        ui->invalid_text->setText("Incorrect symbols in the password!");
        return;
    }
    if(password!=password_again){
        ui->invalid_text->setText("Passwords are not the same.");
        return;
    }

    QJsonObject json;
    json["last_name"]=last_name;
    json["first_name"] = first_name;
    json["middle_name"] = middle_name;
    json["phone_number"] = phone_number;
    json["password"] = password;
    json["user_type"]="patient";
    auth::user_info user = auth::register_user(
        phone_number.toStdString(), password.toStdString(), first_name.toStdString(), last_name.toStdString(), middle_name.toStdString()
        );
    if (user.id != -1) {
        ui->invalid_text->setText("Registration successful!");
    } else {
        ui->invalid_text->setText("Registration failed. Please try again.");
    }
}

