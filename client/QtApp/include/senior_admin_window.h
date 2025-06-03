#ifndef SENIOR_ADMIN_WINDOW_H
#define SENIOR_ADMIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
class SeniorAdminWindow;
}

class SeniorAdminWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SeniorAdminWindow(QWidget *parent = nullptr);
    ~SeniorAdminWindow();
    void set_user_id(int id);
    int get_user_id();
private slots:
    void on_add_junior_administrator_button_clicked();

    void on_add_new_hospital_button_clicked();

    void on_get_users_table_button_clicked();

    void on_get_hospitals_table_button_clicked();

    void make_all_basic();

    void on_button_get_hospitals_clicked();

    void on_button_get_users_clicked();

    void on_button_add_hospital_clicked();

    void on_button_add_junior_admin_clicked();

private:
    Ui::SeniorAdminWindow *ui;
    int user_id = 0;
};

#endif  // SENIORADMINWINDOW_H
