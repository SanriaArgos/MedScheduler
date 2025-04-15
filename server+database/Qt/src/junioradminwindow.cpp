#include "junioradminwindow.h"
#include "ui_junioradminwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <utils.h>
#include <nlohmann/json.hpp>
#include "server+database/client/src/client_junior_admin.cpp"
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
        ui->add_doctor_error->setText("Incorrect format for the middle name. If there is no middle name, enter '-'.");
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
    json["patronymic"] = middle_name;
    json["phone"] = phone_number;
    json["education"] = education;
    json["specialty"] = specialty;
    json["experience"] = experience.toInt();
    QJsonDocument doc(json);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    nlohmann::json doctor_data = nlohmann::json::parse(jsonString.toStdString());

    junior_admin::junior_admin_client client(1);
    client.add_doctor(doctor_data);
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
    json["doctor_id"]=doctor_id.toInt();
    json["hospital_id"]=hospital_id.toInt();
    QJsonDocument qdoc(json);
    nlohmann::json doctor_data = nlohmann::json::parse(qdoc.toJson().toStdString());
    junior_admin::junior_admin_client client(1);
    client.attach_doctor_to_hospital_class(doctor_data);
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
    QString appointment_time = ui->time_form->text();
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
    if(!is_valid_time(appointment_time)){
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
    json["doctor_id"]=doctor_id.toInt();
    json["date"]=date;
    json["time"]=appointment_time;
    json["hospital_id"]=hospital_id.toInt();
    json["cabinet"]=cabinet.toInt();
    json["junior_admin_id"]=1;
    QJsonDocument qdoc(json);
    nlohmann::json data = nlohmann::json::parse(qdoc.toJson().toStdString());
    junior_admin::junior_admin_client client(1);
    client.add_record_slot(data);
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
    junior_admin::junior_admin_client client(1);
    nlohmann::json full_response  = client.get_doctor_schedule(doctor_id.toInt());
    QJsonDocument qdoc = QJsonDocument::fromJson(QByteArray::fromStdString(full_response.dump()));
qDebug().noquote() << qdoc.toJson(QJsonDocument::Indented);
    
}


void JuniorAdminWindow::on_get_users_table_button_clicked()
{
    junior_admin::junior_admin_client client(1);
    nlohmann::json users = client.get_users_table();
    std::string jsonString = users.dump();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    // Преобразуем данные в JSON-документ


    // Получаем корневой объект JSON
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("users") || !rootObject["users"].isArray()) {
        qDebug() << "Ключ 'users' отсутствует или не является массивом!";
        return;
    }
    QJsonArray usersArray = rootObject["users"].toArray();
    QWidget *contentWidget = new QWidget(ui->users_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    // Добавляем данные в виджет
    for (const QJsonValue &userValue : usersArray) {
        if (userValue.isObject()) {
            QJsonObject userObj = userValue.toObject();
            QString id=userObj["id"].toString();

            QString fullName = userObj["last_name"].toString() + ", " +
                               userObj["first_name"].toString() + ", " +
                               userObj["patronymic"].toString();

            QString phoneNumber = userObj["phone"].toString();

            QString type= userObj["user_type"].toString();

            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(id+", "+fullName +", "+ phoneNumber+", "+type);
            label->setStyleSheet("border: 1px solid black; padding: 5px;");

            // Добавляем QLabel в layout
            layout->addWidget(label);
        }
    }
    // Устанавливаем layout для contentWidget
    contentWidget->setLayout(layout);
    // Устанавливаем contentWidget в QScrollArea
    ui->users_scroll->setWidget(contentWidget);
}


void JuniorAdminWindow::on_get_doctors_table_button_clicked()
{
    junior_admin::junior_admin_client client(1);
    nlohmann::json users = client.get_doctors_table();
    std::string jsonString = users.dump();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    // Преобразуем данные в JSON-документ

    // Получаем корневой объект JSON
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("doctors") || !rootObject["doctors"].isArray()) {
        qDebug() << "Ключ 'users' отсутствует или не является массивом!";
        return;
    }
    QJsonArray usersArray = rootObject["doctors"].toArray();
    QWidget *contentWidget = new QWidget(ui->users_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    // Добавляем данные в виджет
    for (const QJsonValue &userValue : usersArray) {
        if (userValue.isObject()) {
            QJsonObject userObj = userValue.toObject();
            QString id=userObj["doctor_id"].toString()+", "+userObj["user_id"].toString();

            QString fullName = userObj["last_name"].toString() + ", " +
                               userObj["first_name"].toString() + ", " +
                               userObj["patronymic"].toString();

            QString phoneNumber = userObj["phone"].toString();
            QString education = userObj["education"].toString();
            QString specialty = userObj["specialty"].toString();
            QString experience = userObj["experience"].toString();

            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(id+", "+fullName +", "+ phoneNumber+", "+education +", "+specialty+", "+experience);
            label->setStyleSheet("border: 1px solid black; padding: 5px;");

            // Добавляем QLabel в layout
            layout->addWidget(label);
        }
    }
    // Устанавливаем layout для contentWidget
    contentWidget->setLayout(layout);
    // Устанавливаем contentWidget в QScrollArea
    ui->doctors_scroll->setWidget(contentWidget);
}
