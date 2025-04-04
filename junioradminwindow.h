#ifndef JUNIORADMINWINDOW_H
#define JUNIORADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class JuniorAdminWindow;
}

class JuniorAdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JuniorAdminWindow(QWidget *parent = nullptr);
    ~JuniorAdminWindow();

private slots:
    void on_add_doctor_button_clicked();

    void on_add_doctor_to_hospital_button_clicked();

    void on_remove_doctor_button_clicked();

    void on_add_appointment_button_clicked();

    void on_get_schedule_button_clicked();

    void on_get_users_table_button_clicked();

private:
    Ui::JuniorAdminWindow *ui;
};

#endif // JUNIORADMINWINDOW_H
