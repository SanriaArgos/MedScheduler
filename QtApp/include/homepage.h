#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDate>
#include <QMainWindow>
#include <QString>
#include <QTime>
#include "qboxlayout.h"

namespace Ui {
class homepage;
}

struct Doctor {
    QString name = "-";
    QString specialty = "-";
    int price = 0;
    int doctor_id = 0;
    int experience = 0;
    double rating = 0.0;
};

struct Record {
    QString admin_phone;
    QDate appointment_date;
    QTime appointment_time;
    QString doctor_name;
    QString hospital_name;
    QString house;
    QString price;
    QString region;
    QString settlement_name;
    QString settlement_type;
    QString specialty;
    QString street;
    int record_id;
};

class homepage : public QMainWindow {
    Q_OBJECT

public:
    explicit homepage(QWidget *parent = nullptr);
    ~homepage();
    void set_user_id(int id);
    int get_user_id();
    std::vector<Record> all_records;
private slots:
    void on_appointments_button_clicked();

    void on_profile_button_clicked();

    void on_doctors_button_clicked();

    void on_hospitals_button_clicked();

    void on_notifications_button_clicked();

    void on_settings_button_clicked();

    void make_all_basic();

    void on_apply_filtres_button_clicked();

    void on_edit_profile_button_clicked();

    void on_apply_changes_edit_clicked();

    void on_delete_account_button_clicked();

    void on_upcoming_button_clicked();

    void on_all_button_clicked();

    void on_completed_button_clicked();

    void fill_appointments_scroll(const std::vector<Record> &records);

    void sort_records(std::vector<Record> &recs, bool newestFirst);

    void on_back_to_doctors_clicked();

private:
    void create_doctor_card(const Doctor &doctor, QVBoxLayout *layout);
    Ui::homepage *ui;
    int user_id = 0;
};

void create_doctor_card(const Doctor &doctor, QVBoxLayout *layout);
#endif  // HOMEPAGE_H
