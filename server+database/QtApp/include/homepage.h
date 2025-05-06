#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include "qboxlayout.h"

namespace Ui {
class homepage;
}

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

    void on_combo_box_region_currentTextChanged(const QString &arg1);

    void on_combo_box_settlement_type_currentTextChanged(const QString &arg1);

    void on_combo_box_settlement_name_currentTextChanged(const QString &arg1);

    void on_combo_box_specialty_currentTextChanged(const QString &arg1);

    void on_apply_filtres_button_clicked();

    void on_combo_box_hospital_currentTextChanged(const QString &arg1);

private:
    Ui::homepage *ui;
    int user_id = 0;
};
struct Doctor {
    QString last_name;
    QString first_name;
    QString patronym;
    QString specialty;
    QString hospital_name;
    QString education;
    int price;
    int doctor_id;
    int experience;
    double rating;
};
void create_doctor_card(const Doctor &doctor, QVBoxLayout *layout);
#endif  // HOMEPAGE_H
