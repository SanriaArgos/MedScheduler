#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
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

class homepage : public QMainWindow {
    Q_OBJECT

public:
    explicit homepage(QWidget *parent = nullptr);
    ~homepage();
    void set_user_id(int id);
    int get_user_id();
private slots:
    void on_appointments_button_clicked();

    void on_profile_button_clicked();

    void on_doctors_button_clicked();

    void on_hospitals_button_clicked();

    void on_notifications_button_clicked();

    void on_settings_button_clicked();

    void make_all_basic();

    void on_apply_filtres_button_clicked();

private:
    void create_doctor_card(const Doctor &doctor, QVBoxLayout *layout);
    Ui::homepage *ui;
    int user_id = 0;
};

void create_doctor_card(const Doctor &doctor, QVBoxLayout *layout);
#endif  // HOMEPAGE_H
