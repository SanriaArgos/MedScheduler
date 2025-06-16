#include "junior_admin_window.h"
#include <utils.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <nlohmann/json.hpp>
#include "client_junior_admin.hpp"
#include "ui_junior_admin_window.h"

JuniorAdminWindow::JuniorAdminWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::JuniorAdminWindow) {
    ui->setupUi(this);
    make_all_basic();
    on_button_get_users_clicked();
}

void JuniorAdminWindow::set_user_id(int id) {
    user_id = id;
}

int JuniorAdminWindow::get_user_id() {
    return user_id;
}

JuniorAdminWindow::~JuniorAdminWindow() {
    delete ui;
}

void JuniorAdminWindow::on_add_doctor_button_clicked() {
    ui->add_doctor_error->setText("");
    QString last_name = ui->last_name_form->text();
    QString first_name = ui->first_name_form->text();
    QString middle_name = ui->middle_name_form->text();
    QString phone_number = ui->phone_number_form->text();
    QString education = ui->education_form->text();
    QString specialty = ui->specialty_form->text();
    QString experience = ui->experience_form->text();
    QString price = ui->price_form->text();

    if (!is_latin_or_dash(last_name)) {
        ui->add_doctor_error->setText("Incorrect format for the last name.");
        return;
    }

    if (!is_latin_or_dash(first_name)) {
        ui->add_doctor_error->setText("Incorrect format for the first name.");
        return;
    }

    if (!is_latin_or_dash(middle_name)) {
        ui->add_doctor_error->setText(
            "Incorrect format for the middle name. If there is no middle name, "
            "enter '-'."
        );
        return;
    }

    if (!is_validated_phone(phone_number)) {
        ui->add_doctor_error->setText("Incorrect format for the phone number.");
        return;
    }

    if (!is_latin_or_dash(specialty)) {
        ui->add_doctor_error->setText("Incorrect format for specialty.");
        return;
    }

    if (!is_number(experience)) {
        ui->add_doctor_error->setText("Incorrect format for experience.");
        return;
    }
    if (!is_number(price)) {
        ui->add_doctor_error->setText("Incorrect format for price.");
        return;
    }

    QJsonObject json;
    json["last_name"] = last_name;
    json["first_name"] = first_name;
    json["patronymic"] = middle_name;
    json["phone"] = phone_number;
    json["education"] = education;
    json["specialty"] = specialty;
    json["experience"] = experience.toInt();
    json["price"] = price.toInt();

    QJsonDocument doc(json);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    nlohmann::json doctor_data =
        nlohmann::json::parse(jsonString.toStdString());

    junior_admin::junior_admin_client client(get_user_id());
    client.add_doctor(doctor_data);
}

void JuniorAdminWindow::on_add_doctor_to_hospital_button_clicked() {
    ui->error_label_add_remove->setText("");
    QString doctor_id = ui->doctor_id_form->text();
    QString hospital_id = ui->hospital_id_form->text();
    if (!is_number(doctor_id)) {
        ui->error_label_add_remove->setText("Incorrect format for doctor id.");
        return;
    }
    if (!is_number(hospital_id)) {
        ui->error_label_add_remove->setText("Incorrect format for hospital id."
        );
        return;
    }
    QJsonObject json;
    json["doctor_id"] = doctor_id.toInt();
    json["hospital_id"] = hospital_id.toInt();
    QJsonDocument qdoc(json);
    nlohmann::json doctor_data =
        nlohmann::json::parse(qdoc.toJson().toStdString());
    junior_admin::junior_admin_client client(get_user_id());
    client.attach_doctor_to_hospital_class(doctor_data);
}

void JuniorAdminWindow::on_remove_doctor_button_clicked() {
    ui->error_label_add_remove->setText("");
    QString doctor_id = ui->doctor_id_form->text();
    QString hospital_id = ui->hospital_id_form->text();

    if (!is_number(doctor_id)) {
        ui->error_label_add_remove->setText("Incorrect format for doctor id.");
        return;
    }
    if (!is_number(hospital_id)) {
        ui->error_label_add_remove->setText("Incorrect format for hospital id."
        );
        return;
    }

    QJsonObject json;
    json["doctor_id"] = doctor_id.toInt();
    json["hospital_id"] = hospital_id.toInt();
    json["junior_admin_id"] = get_user_id();

    QJsonDocument qdoc(json);
    nlohmann::json doctor_data =
        nlohmann::json::parse(qdoc.toJson().toStdString());

    junior_admin::junior_admin_client client(get_user_id());
    client.detach_doctor_from_hospital(doctor_data);  // ✅ вызов нужной функции
}

void JuniorAdminWindow::on_add_appointment_button_clicked()  // TODO
{
    ui->error_appointment->setText("");
    QString doctor_id = ui->doctor_id_form_from_appointment->text();
    QString date = ui->date_form->text();
    QString timee = ui->time_form->text();
    QString hospital_id = ui->hospital_id_form_from_appointment->text();
    QString cabinet = ui->cabinet_form->text();
    if (!is_number(doctor_id)) {
        ui->error_appointment->setText("Incorrect format for doctor id.");
        return;
    }
    if (!is_valid_date(date)) {
        ui->error_appointment->setText("Incorrect format for date(YYYY-MM-DD)."
        );
        return;
    };
    if (!is_valid_time(timee)) {
        ui->error_appointment->setText("Incorrect format for time(HH:MM).");
        return;
    };
    if (!is_number(hospital_id)) {
        ui->error_appointment->setText("Incorrect format for hospital id.");
        return;
    }
    if (!is_number(cabinet)) {
        ui->error_appointment->setText("Incorrect format for cabinet.");
        return;
    }
    QJsonObject json;
    json["doctor_id"] = doctor_id.toInt();
    json["date"] = date;
    json["time"] = timee;
    json["hospital_id"] = hospital_id.toInt();
    json["cabinet"] = cabinet.toInt();
    json["junior_admin_id"] = get_user_id();
    QJsonDocument qdoc(json);
    nlohmann::json data = nlohmann::json::parse(qdoc.toJson().toStdString());
    junior_admin::junior_admin_client client(get_user_id());
    client.add_record_slot(data);
}

void JuniorAdminWindow::on_get_schedule_button_clicked()  // TODO
{
    ui->error_schedule->setText("");
    QString doctor_id = ui->doctor_id_schedule_form->text();

    if (!is_number(doctor_id)) {
        ui->error_schedule->setText("Incorrect format for doctor id.");
        return;
    }

    junior_admin::junior_admin_client client(get_user_id());
    nlohmann::json full_response =
        client.get_doctor_schedule(doctor_id.toInt());
    std::string jsonString = full_response.dump();
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    QJsonObject rootObject = jsonDoc.object();
    qDebug() << "Full JSON:" << jsonDoc.toJson(QJsonDocument::Indented);

    if (!rootObject.contains("schedule") || !rootObject["schedule"].isArray()) {
        qDebug() << "Ключ 'schedule' отсутствует или не является массивом!";
        return;
    }
    QJsonArray scheduleArray = rootObject["schedule"].toArray();
    if (scheduleArray.size() == 0) {
        ui->error_schedule->setText("Schedule is empty!");
    }
    QWidget *contentWidget = new QWidget(ui->schedule_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    for (const QJsonValue &entry : scheduleArray) {
        if (entry.isObject()) {
            QJsonObject userObj = entry.toObject();
            QString date = userObj["appointment_date"].toString();
            QString time = userObj["appointment_time"].toString();
            QString cabinet = userObj["cabinet_number"].toString();
            QString hospital_name = userObj["full_name"].toString();
            QString first_name = userObj["first_name"].toString();
            QString last_name = userObj["last_name"].toString();
            QString patronymic = userObj["patronymic"].toString();
            QString phone = userObj["phone"].toString();
            first_name = (first_name == "") ? "-" : first_name;
            last_name = (last_name == "") ? "-" : last_name;
            patronymic = (patronymic == "") ? "-" : patronymic;
            phone = (phone == "") ? "-" : phone;
            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(
                date + ", " + time + ", " + cabinet + ", " + hospital_name +
                ", " + first_name + ", " + last_name + ", " + patronymic +
                ", " + phone
            );
            label->setStyleSheet("border: 1px solid black; padding: 5px;");

            // Добавляем QLabel в layout
            layout->addWidget(label);
        }
    }
    // Устанавливаем layout для contentWidget
    contentWidget->setLayout(layout);
    // Устанавливаем contentWidget в QScrollArea
    ui->schedule_scroll->setWidget(contentWidget);
}

void JuniorAdminWindow::on_get_users_table_button_clicked() {
    junior_admin::junior_admin_client client(get_user_id());
    nlohmann::json users = client.get_users_table();
    std::string jsonString = users.dump();
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
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
    ui->users_scroll->setWidget(contentWidget);
}

void JuniorAdminWindow::on_get_doctors_table_button_clicked() {
    junior_admin::junior_admin_client client(get_user_id());
    nlohmann::json users = client.get_doctors_table();
    std::string jsonString = users.dump();
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray::fromStdString(jsonString));
    // Преобразуем данные в JSON-документ

    // Получаем корневой объект JSON
    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("doctors") || !rootObject["doctors"].isArray()) {
        qDebug() << "Ключ 'doctors' отсутствует или не является массивом!";
        return;
    }
    QJsonArray usersArray = rootObject["doctors"].toArray();
    QWidget *contentWidget = new QWidget(ui->doctors_scroll);
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    // Добавляем данные в виджет
    for (const QJsonValue &userValue : usersArray) {
        if (userValue.isObject()) {
            QJsonObject userObj = userValue.toObject();
            QString id = userObj["doctor_id"].toString() + ", " +
                         userObj["user_id"].toString();

            QString fullName = userObj["last_name"].toString() + ", " +
                               userObj["first_name"].toString() + ", " +
                               userObj["patronymic"].toString();

            QString phoneNumber = userObj["phone"].toString();
            QString education = userObj["education"].toString();
            QString specialty = userObj["specialty"].toString();
            QString experience = userObj["experience"].toString();
            QString price = userObj["price"].toString();

            // Создаем QLabel для отображения данных
            QLabel *label = new QLabel(contentWidget);
            label->setText(
                id + ", " + fullName + ", " + phoneNumber + ", " + education +
                ", " + specialty + ", " + experience + ", " + price
            );
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

void JuniorAdminWindow::make_all_basic() {
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

    ui->button_add_appointment->setStyleSheet(baseStyle);
    ui->button_add_doctor->setStyleSheet(baseStyle);
    ui->button_attach_doctor->setStyleSheet(baseStyle);
    ui->button_get_users->setStyleSheet(baseStyle);
    ui->button_get_schedule->setStyleSheet(baseStyle);
    ui->button_get_doctors->setStyleSheet(baseStyle);
    ui->button_edit_profile->setStyleSheet(baseStyle);
    ui->button_get_statistics->setStyleSheet(baseStyle);
    ui->button_get_waitlist->setStyleSheet(baseStyle);
}

void JuniorAdminWindow::on_button_get_users_clicked() {
    on_get_users_table_button_clicked();
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

void JuniorAdminWindow::on_button_get_doctors_clicked() {
    on_get_doctors_table_button_clicked();
    ui->stackedWidget->setCurrentWidget(ui->page_get_doctors);
    make_all_basic();
    ui->button_get_doctors->setStyleSheet(
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

void JuniorAdminWindow::on_button_add_doctor_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_add_doctor);
    make_all_basic();
    ui->button_add_doctor->setStyleSheet(
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

void JuniorAdminWindow::on_button_attach_doctor_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_attach_doctor);
    make_all_basic();
    ui->button_attach_doctor->setStyleSheet(
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

void JuniorAdminWindow::on_button_add_appointment_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_add_appointment);
    make_all_basic();
    ui->button_add_appointment->setStyleSheet(
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

void JuniorAdminWindow::on_button_get_schedule_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_get_schedule);
    make_all_basic();
    ui->button_get_schedule->setStyleSheet(
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

void JuniorAdminWindow::on_edit_profile_button_clicked()
{
    ui->profile_edit_error->setText("");
    QString first_name = ui->edit_first_name->text();
    QString last_name = ui->edit_last_name->text();
    QString middle_name = ui->edit_middle_name->text();
    QString new_password = ui->edit_new_password->text();
    QString new_password_again = ui->edit_new_password_again->text();
    QString current_password = ui->current_password->text();
    if (first_name != "" && !is_latin_or_dash(first_name)) {
        ui->profile_edit_error->setText("Incorrect formar for first name");
        return;
    }
    if (last_name != "" && !is_latin_or_dash(last_name)) {
        ui->profile_edit_error->setText("Incorrect formar for last name");
        return;
    }
    if (middle_name != "" && !is_latin_or_dash(middle_name)) {
        ui->profile_edit_error->setText("Incorrect formar for middle name");
        return;
    }
    if (new_password != "" && !is_valid_password(new_password)) {
        ui->profile_edit_error->setText("Incorrect formar for new password");
        return;
    }
    if (new_password != "" && new_password_again == "") {
        ui->profile_edit_error->setText("Enter your new password again");
        return;
    }
    if (new_password != "" && new_password != new_password_again) {
        ui->profile_edit_error->setText("New passwords do not match");
        return;
    }
    if (current_password == "") {
        ui->profile_edit_error->setText("Enter your current password");
        return;
    }
    if (!is_valid_password(current_password)) {
        ui->profile_edit_error->setText("Incorrect format for current password");
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
    if (new_password != "") {
        json["new_password"] = new_password;
        json["new_password_repeat"] = new_password_again;
    }
    QJsonDocument doc(json);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    nlohmann::json edit_data = nlohmann::json::parse(jsonString.toStdString());
    junior_admin::junior_admin_client client(get_user_id());
    nlohmann::json response = client.edit_junior_admin_profile(edit_data);
    if (response.contains("error")) {
        ui->profile_edit_error->setText(QString::fromStdString(response["error"])
                                      );
    } else if (response.contains("message")) {
        ui->profile_edit_error->setText(QString::fromStdString(response["message"]
                                                             ));
    }
}


void JuniorAdminWindow::on_button_edit_profile_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_edit_profile);
    make_all_basic();
    ui->button_edit_profile->setStyleSheet(
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


void JuniorAdminWindow::on_button_get_statistics_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_get_statistics);
    make_all_basic();
    ui->button_get_statistics->setStyleSheet(
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


void JuniorAdminWindow::on_submit_stats_clicked()
{
    QString phone = ui->doctor_phone_lineedit->text();
    if (!is_validated_phone(phone)) {
        ui->rating_label->setText("Incorrect format for the phone number.");
        return;
    }
    nlohmann::json j;
    j["doctor_phone"]=phone.toStdString();
    j["junior_admin_id"]=get_user_id();
    junior_admin::junior_admin_client client(get_user_id());
    nlohmann::json response = client.get_doctor_statistics(j);
    if (response.contains("success") && response["success"]==true){
    ui->price_doctor_label->setText(QString::number(response["price"].get<double>()));
    ui->rating_label->setText(QString::number(response["average_rating"].get<double>()));
    ui->count_label->setText(QString::number(response["patients_count"].get<int>()));
    }
}


void JuniorAdminWindow::on_get_waitlist_button_clicked()
{
    bool ok; // Флаг успешности преобразования
    int doctor_id = ui->doctor_id_for_waitlist_edit->text().toInt(&ok);
    if (!ok) {
        return;
    }
    junior_admin::junior_admin_client client(get_user_id());
    nlohmann::json response = client.get_waitlist(doctor_id,get_user_id());
    if (ui->scrollArea->widget()) {
        delete ui->scrollArea->widget();
    }

    // Создаем контейнер для элементов
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    container->setLayout(layout);

    // Проходим по всем элементам waitlist
    for (const auto& patient : response["waitlist"]) {
        // Создаем виджет для одного пациента
        QFrame *patientFrame = new QFrame();
        patientFrame->setFrameShape(QFrame::StyledPanel);


        QVBoxLayout *patientLayout = new QVBoxLayout(patientFrame);

                // Добавляем данные пациента
        QString patientInfo = QString("%1 %2 %3\nID: %4\nPhone: %5\nDate: %6")
                                  .arg(QString::fromStdString(patient["last_name"]))
                                  .arg(QString::fromStdString(patient["first_name"]))
                                  .arg(QString::fromStdString(patient["patronymic"]))
                                  .arg(patient["patient_id"].get<int>())
                                  .arg(QString::fromStdString(patient["phone"]))
                                  .arg(QString::fromStdString(patient["request_date"]));

        QLabel *infoLabel = new QLabel(patientInfo);
        patientLayout->addWidget(infoLabel);

                // Добавляем виджет пациента в общий layout
        layout->addWidget(patientFrame);
    }

    // Добавляем растягивающий элемент, чтобы виджеты не растягивались на всю высоту
    layout->addStretch();

    // Устанавливаем контейнер в ScrollArea
    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);
}


void JuniorAdminWindow::on_button_get_waitlist_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_waitlist);
    make_all_basic();
    ui->button_get_waitlist->setStyleSheet(
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

