#ifndef JUNIOR_ADMIN_WINDOW_H
#define JUNIOR_ADMIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
class JuniorAdminWindow;
}

class JuniorAdminWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit JuniorAdminWindow(QWidget *parent = nullptr);
    ~JuniorAdminWindow();
    void set_user_id(int id);
    int get_user_id();
private slots:
    void on_add_doctor_button_clicked();

    void on_add_doctor_to_hospital_button_clicked();

    void on_remove_doctor_button_clicked();

    void on_add_appointment_button_clicked();

    void on_get_schedule_button_clicked();

    void on_get_users_table_button_clicked();

    void on_get_doctors_table_button_clicked();

private:
    Ui::JuniorAdminWindow *ui;
    int user_id = 0;
};

#endif  // JUNIORADMINWINDOW_H
