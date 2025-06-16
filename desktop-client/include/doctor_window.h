#ifndef DOCTOR_WINDOW_H
#define DOCTOR_WINDOW_H

#include <QMainWindow>

namespace Ui {
class DoctorWindow;
}

class DoctorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DoctorWindow(QWidget *parent = nullptr);
    ~DoctorWindow();
    void set_user_id(int id);
    int get_user_id();
private slots:
    void on_view_schedule_button_clicked();

    void on_pushButton_clicked();

private:
    Ui::DoctorWindow *ui;
    int user_id = 0;
};

#endif  // DOCTOR_WINDOW_H
