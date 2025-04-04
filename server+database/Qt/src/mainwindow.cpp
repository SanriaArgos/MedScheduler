#include "../include/mainwindow.h"
#include "../include/ui_mainwindow.h"
#include "../include/homepage.h"
#include "../include/registrationwindow.h"
#include "../include/senioradminwindow.h"
#include "../include/junioradminwindow.h"
#include <include/utils.h>
#include <QPixmap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

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
    QString number = ui->login_input->text();
    QString password = ui->password_input->text();
    if(!is_validated_phone(number)){
        ui->near_enter->setText("Incorrect format for the phone number!");
        return;
    }
    if (password.size() < 8) {
        ui->near_enter->setText("Must be at least 8 symbols in the password!");
        return;
    }
    if(!is_valid_password(password)){
        ui->near_enter->setText("Incorrect symbols in the password!");
        return;
    }
    QJsonObject json;
    json["phone_number"]= number;
    json["password"]= password;
    homepage *w = new homepage(); // Создаем окно динамически
    w->setAttribute(Qt::WA_DeleteOnClose); // Убедимся, что окно удалится при закрытии
    w->showMaximized(); // Показываем окно
    // Закрытие текущего окна
    this->close();
}

void MainWindow::on_registration_button_clicked()
{
    RegistrationWindow window;
    window.setModal(true);
    window.exec();
}


void MainWindow::on_junior_button_clicked()
{
    JuniorAdminWindow *w = new JuniorAdminWindow(); // Создаем окно динамически
    w->setAttribute(Qt::WA_DeleteOnClose); // Убедимся, что окно удалится при закрытии
    w->show(); // Показываем окно
    // Закрытие текущего окна
    this->close();
}


void MainWindow::on_pushButton_2_clicked()
{
    SeniorAdminWindow*w = new SeniorAdminWindow(); // Создаем окно динамически
    w->setAttribute(Qt::WA_DeleteOnClose); // Убедимся, что окно удалится при закрытии
    w->show(); // Показываем окно
    // Закрытие текущего окна
    this->close();
}

