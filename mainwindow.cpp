#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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

void MainWindow::on_pushButton_clicked()
{
    if(ui->radioButton->isChecked()){
        ui->statusbar->showMessage("Вы пациент");
    }
    else if(ui->radioButton_2->isChecked()){
        ui->statusbar->showMessage("Вы врач");
    }
    else{
        ui->statusbar->showMessage("я не знаю кто вы");
    }

}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()){
        QMessageBox::warning(this, "Неееет","Убери галочку!");
    }
    else{
        ui->statusbar->showMessage("молодец");
    }
}

