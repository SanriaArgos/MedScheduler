#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QWidget>
#include <QDate>
#include <QTime>
#include <QPushButton>
namespace Ui {
class Appointment;
}

class Appointment : public QWidget {
    Q_OBJECT

public:
    explicit Appointment(QWidget *parent = nullptr);
    ~Appointment();
    int doctor_id;
    int user_id;

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
    void handle_day_clicked(QPushButton* clicked_button, int days);
};

struct slot{
    QDate appointment_date;
    QTime appointment_time;
    QString full_name;
    QString house;
    QString junior_admin_phone;
    QString region;
    QString settlement_name;
    QString settlement_type;
    QString slot_status;
    QString street;
};

#endif  // APPOINTMENT_H
