#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QWidget>
#include <QDate>
#include <QTime>
namespace Ui {
class Appointment;
}

class Appointment : public QWidget {
    Q_OBJECT

public:
    explicit Appointment(QWidget *parent = nullptr);
    ~Appointment();
    int doctor_id;

private slots:
    void on_date0_clicked();

    void on_date1_clicked();

    void on_date2_clicked();

    void on_date3_clicked();

    void on_date4_clicked();

    void on_date5_clicked();

    void on_date6_clicked();

    void make_all_basic();

private:
    Ui::Appointment *ui;
};

struct slot{
    QDate date;
    QTime time;
    QString hospital;
    QString adress;
    int cabinet;
};

#endif  // APPOINTMENT_H
