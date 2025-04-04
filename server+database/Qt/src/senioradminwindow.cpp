#include "../include/utils.h"
#include "../include/senioradminwindow.h"
#include "../include/ui_senioradminwindow.h"
#include "../../client/include/client_senior_admin.hpp"
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "../../client/include/client_senior_admin.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../client/include/common_for_all.hpp"


SeniorAdminWindow::SeniorAdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SeniorAdminWindow)
{
    ui->setupUi(this);
}

SeniorAdminWindow::~SeniorAdminWindow()
{
    delete ui;
}

void SeniorAdminWindow::on_add_junior_administrator_button_clicked()
{
    ui->error_add_junior->setText("");
    QString last_name=ui->last_name_form->text();
    QString first_name=ui->first_name_form->text();
    QString middle_name=ui->middle_name_form->text();
    QString phone_number=ui->phone_number_form->text();
    if(!is_latin_or_dash(last_name)){
        ui->error_add_junior->setText("Incorrect format for the last name.");
        return;
    }
    if(!is_latin_or_dash(first_name)){
        ui->error_add_junior->setText("Incorrect format for the first name.");
        return;
    }
    if(!is_latin_or_dash(middle_name)){
        ui->error_add_junior->setText("Incorrect format for the middle name.");
        return;
    }
    if(!is_validated_phone(phone_number)){
        ui->error_add_junior->setText("Incorrect format for the phone number.");
        return;
    }

    QJsonObject junior_admin_data = {
                {"last_name", last_name},
                {"first_name", first_name},
                {"patronymic", middle_name},
                {"phone", phone_number}};
    // Преобразование QJsonObject в строку, если функция add_junior_admin принимает std::string
    QByteArray jsonData = QJsonDocument(junior_admin_data).toJson();
    std::string jsonString = jsonData.toStdString();
    nlohmann::json j = nlohmann::json::parse(jsonString);

    senior_admin::senior_admin_client client(1);
    client.add_junior_admin(j);
}


void SeniorAdminWindow::on_add_new_hospital_button_clicked()
{
    ui->error_add_hospital->setText("");
    QString region=ui->region_form->text();
    QString settlement_type= ui->settlement_type_form->text();
    QString settlement_name=ui->settlement_name_form->text();
    QString street = ui->street_form->text();
    QString house = ui->house_form->text();
    QString hospital_full_name =ui->hospital_full_name_form->text();
    QString id_of_junior_administrator =ui->id_of_junior_administrator_form->text();
    if(!is_latin_or_dash(region)){
        ui->error_add_hospital->setText("Incorrect format for the region.");
        return;
    }
    if(!is_latin_or_dash(settlement_type)){
        ui->error_add_hospital->setText("Incorrect format for the settlement type.");
        return;
    }
    if(!is_latin_or_dash(settlement_name)){
        ui->error_add_hospital->setText("Incorrect format for the settlement name.");
        return;
    }
    if(!is_latin_or_dash(street)){
        ui->error_add_hospital->setText("Incorrect format for the street.");
        return;
    }
    if(!is_number(house)){
        ui->error_add_hospital->setText("Incorrect format for the house.");
        return;
    }
    if(!is_number(id_of_junior_administrator)){
        ui->error_add_hospital->setText("Incorrect format for id of junior administrator.");
        return;
    }
    QJsonObject json;
    json["region"]=region;
    json["settlement_type"] = settlement_type;
    json["settlement_name"] = settlement_name;
    json["street"] = street;
    json["house"]=house;
    json["hospital_full_name"]=hospital_full_name;
    json["id_of_junior_administrator"]=id_of_junior_administrator;
    //send
}

