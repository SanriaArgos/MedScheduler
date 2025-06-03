#include "main_window.h"
#include <utils.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QString>
#include "client_auth.hpp"
#include "client_doctor.hpp"
#include "client_junior_admin.hpp"
#include "client_patient.hpp"
#include "client_senior_admin.hpp"
#include "doctor_window.h"
#include "homepage.h"
#include "junior_admin_window.h"
#include "registration_window.h"
#include "senior_admin_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_enter_clicked() {
    ui->near_enter->setText("");
    QString number = ui->login_input->text();
    QString password = ui->password_input->text();
    if (!is_validated_phone(number)) {
        ui->near_enter->setText("Incorrect format for the phone number!");
        return;
    }
    if (password.size() < 8) {
        ui->near_enter->setText("Must be at least 8 symbols in the password!");
        return;
    }
    if (!is_valid_password(password)) {
        ui->near_enter->setText("Incorrect symbols in the password!");
        return;
    }
    QJsonObject json;
    json["phone_number"] = number;
    json["password"] = password;
    auth::user_info user =
        auth::login(number.toStdString(), password.toStdString());
    if (user.id != -1) {
        ui->near_enter->setText("Login successful!");
        if (user.user_type == "doctor") {
            DoctorWindow *w = new DoctorWindow;
            w->set_user_id(user.id);
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->show();
            this->close();
        } else if (user.user_type == "junior administrator") {
            JuniorAdminWindow *w =
                new JuniorAdminWindow();  // Создаем окно динамически
            w->set_user_id(user.id);
            w->setAttribute(Qt::WA_DeleteOnClose
            );  // Убедимся, что окно удалится при закрытии
            w->show();  // Показываем окно
            // Закрытие текущего окна
            this->close();
        } else if (user.user_type == "senior administrator") {
            SeniorAdminWindow *w =
                new SeniorAdminWindow();  // Создаем окно динамически
            w->set_user_id(user.id);
            w->setAttribute(Qt::WA_DeleteOnClose
            );  // Убедимся, что окно удалится при закрытии
            w->show();  // Показываем окно
            // Закрытие текущего окна
            this->close();
        } else if (user.user_type == "patient") {
            homepage *w = new homepage();  // Создаем окно динамически
            w->set_user_id(user.id);
            w->setAttribute(Qt::WA_DeleteOnClose
            );  // Убедимся, что окно удалится при закрытии
            w->show();  // Показываем окно
            // Закрытие текущего окна
            this->close();
        } else {
            ui->near_enter->setText("Unknown user type.");
        }
    } else {
        ui->near_enter->setText("Login failed!");
    }
}

void MainWindow::on_registration_button_clicked() {
    RegistrationWindow window;
    window.setModal(true);
    window.exec();
}
