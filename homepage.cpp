#include "homepage.h"
#include "ui_homepage.h"

homepage::homepage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homepage)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->appointments_page);
}

homepage::~homepage()
{
    delete ui;
}

void homepage::on_appointments_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->appointments_page);
}


void homepage::on_profile_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->profile_page);
}


void homepage::on_doctors_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->doctors_page);
}


void homepage::on_hospitals_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->hospitals_page);
}


void homepage::on_notifications_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->notifications_page);
}


void homepage::on_settings_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->settings_page);
}

