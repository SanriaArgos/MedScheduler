#include "junioradminwindow.h"
#include "ui_junioradminwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <utils.h>
JuniorAdminWindow::JuniorAdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::JuniorAdminWindow)
{
    ui->setupUi(this);
}

JuniorAdminWindow::~JuniorAdminWindow()
{
    delete ui;
}

void JuniorAdminWindow::on_add_doctor_button_clicked()
{
    ui->add_doctor_error->setText("");
    QString last_name=ui->last_name_form->text();
    QString first_name = ui->first_name_form->text();
    QString middle_name = ui->middle_name_form->text();
    QString phone_number = ui->phone_number_form->text();
    QString education = ui->education_form->text();
    QString specialty = ui->specialty_form->text();
    QString experience = ui->experience_form->text();
    if(!is_latin_or_dash(last_name)){
        ui->add_doctor_error->setText("Incorrect format for the last name.");
        return;
    }
    if(!is_latin_or_dash(first_name)){
        ui->add_doctor_error->setText("Incorrect format for the first name.");
        return;
    }
    if(!is_latin_or_dash(middle_name)){
        ui->add_doctor_error->setText("Incorrect format for the middle name.");
        return;
    }
    if(!is_validated_phone(phone_number)){
        ui->add_doctor_error->setText("Incorrect format for the phone number.");
        return;
    }
    if(!is_latin_or_dash(specialty)){
        ui->add_doctor_error->setText("Incorrect format for specialty.");
        return;
    }
    if(!is_number(experience)){
        ui->add_doctor_error->setText("Incorrect format for experience.");
        return;
    }
    QJsonObject json;
    json["last_name"]=last_name;
    json["first_name"] = first_name;
    json["middle_name"] = middle_name;
    json["phone_number"] = phone_number;
    json["education"] = education;
    json["specialty"] = specialty;
    json["experience"] = experience;
    //как-то отправить потом
}


void JuniorAdminWindow::on_add_doctor_to_hospital_button_clicked()
{
    ui->error_label_add_remove->setText("");
    QString doctor_id=ui->doctor_id_form->text();
    QString hospital_id=ui->hospital_id_form->text();
    if(!is_number(doctor_id)){
        ui->error_label_add_remove->setText("Incorrect format for doctor id.");
        return;
    }
    if(!is_number(hospital_id)){
        ui->error_label_add_remove->setText("Incorrect format for hospital id.");
        return;
    }
    QJsonObject json;
    json["doctor_id"]=doctor_id;
    json["hospital_id"]=hospital_id;
    //как-то отправить
}


void JuniorAdminWindow::on_remove_doctor_button_clicked()
{
    ui->error_label_add_remove->setText("");
    QString doctor_id=ui->doctor_id_form->text();
    QString hospital_id=ui->doctor_id_form->text();
    if(!is_number(doctor_id)){
        ui->error_label_add_remove->setText("Incorrect format for doctor id.");
        return;
    }
    if(!is_number(hospital_id)){
        ui->error_label_add_remove->setText("Incorrect format for hospital id.");
        return;
    }
    QJsonObject json;
    json["doctor_id"]=doctor_id;
    json["hospital_id"]=hospital_id;
    //send
}


void JuniorAdminWindow::on_add_appointment_button_clicked()
{
    ui->error_appointment->setText("");
    QString doctor_id=ui->doctor_id_form_from_appointment->text();
    QString date = ui->date_form->text();
    QString time = ui->time_form->text();
    QString hospital_id=ui->hospital_id_form_from_appointment->text();
    QString cabinet = ui->cabinet_form->text();
    if(!is_number(doctor_id)){
        ui->error_appointment->setText("Incorrect format for doctor id.");
        return;
    }
    if(!is_valid_date(date)){
        ui->error_appointment->setText("Incorrect format for date(YYYY-MM-DD).");
        return;
    };
    if(!is_valid_time(time)){
        ui->error_appointment->setText("Incorrect format for time(HH:MM).");
        return;
    };
    if(!is_number(hospital_id)){
        ui->error_appointment->setText("Incorrect format for hospital id.");
        return;
    }
    if(!is_number(cabinet)){
        ui->error_appointment->setText("Incorrect format for cabinet.");
        return;
    }
    QJsonObject json;
    json["doctor_id"]=doctor_id;
    json["date"]=date;
    json["time"]=time;
    json["hospital_id"]=hospital_id;
    json["cabinet"]=cabinet;
    //send
}


void JuniorAdminWindow::on_get_schedule_button_clicked()
{
    ui->error_schedule->setText("");
    QString doctor_id=ui->doctor_id_schedule_form->text();
    if(!is_number(doctor_id)){
        ui->error_schedule->setText("Incorrect format for doctor id.");
        return;
    }
    QJsonObject json;
    json["doctor_id"]=doctor_id;
    //send
}

