#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <string>
#include <utils.h>
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
    std::string last_name = (ui->last_name_form->text()).toStdString();
    std::string first_name = (ui->first_name_form->text()).toStdString();
    std::string middle_name = (ui->middle_name_form->text()).toStdString();
    std::string phone_number = (ui->phone_number_form->text()).toStdString();
    std::string password = (ui->password_form->text()).toStdString();
    std::string password_again = (ui->password_again_form->text()).toStdString();
    if(!is_latin_or_dash(last_name)|| last_name.empty()){
        ui->invalid_text->setText("Incorrect format for the last name.");
        return;
    }
    if(!is_latin_or_dash(first_name) || first_name.empty()){
        ui->invalid_text->setText("Incorrect format for the first name.");
        return;
    }
    if(!is_latin_or_dash(middle_name)|| middle_name.empty()){
        ui->invalid_text->setText("Incorrect format for the middle name.");
        return;
    }
    if(!is_validated_phone(phone_number)|| phone_number.empty()){
        ui->invalid_text->setText("Incorrect format for the phone number.");
        return;
    }
    if(password.empty()){
        ui->invalid_text->setText("Enter the password.");
        return;
    }
    if(password!=password_again){
        ui->invalid_text->setText("Passwords are not the same.");
        return;
    }
}

