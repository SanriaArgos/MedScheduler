#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>

namespace Ui {
class homepage;
}

class homepage : public QMainWindow
{
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

    void on_view_schedule_button_clicked();

    void make_all_basic();

private:
    Ui::homepage *ui;
    int user_id = 0;
};

#endif // HOMEPAGE_H
