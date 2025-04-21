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
};

#endif // HOMEPAGE_H
