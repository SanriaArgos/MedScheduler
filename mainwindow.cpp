#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <utils.h>
#include <QPixmap>
#include "registrationwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enter_clicked()
{
    ui->near_enter->setText("");
    QString qt_number = ui->login_input->text();
    std::string number = qt_number.toStdString();
    QString qt_password = ui->password_input->text();
    std::string password = qt_password.toStdString();
    if(!is_validated_phone(number)){
        ui->near_enter->setText("Incorrect format for the phone number");
        return;
    }

}

void MainWindow::on_registration_button_clicked()
{
    RegistrationWindow window;
    window.setModal(true);
    window.exec();
}

