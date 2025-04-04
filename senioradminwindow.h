#ifndef SENIORADMINWINDOW_H
#define SENIORADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SeniorAdminWindow;
}

class SeniorAdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SeniorAdminWindow(QWidget *parent = nullptr);
    ~SeniorAdminWindow();

private slots:
    void on_add_junior_administrator_button_clicked();

    void on_add_new_hospital_button_clicked();

    void on_get_users_table_button_clicked();

    void on_get_hospitals_table_button_clicked();

private:
    Ui::SeniorAdminWindow *ui;
};

#endif // SENIORADMINWINDOW_H
