#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>

namespace Ui {
class DoctorWindow;
}

class DoctorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoctorWindow(QWidget *parent = nullptr);
    ~DoctorWindow();

private:
    Ui::DoctorWindow *ui;
};

#endif // DOCTORWINDOW_H
