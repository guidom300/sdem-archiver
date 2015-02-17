#ifndef SUCCESS_H
#define SUCCESS_H

#include <QDialog>
#include <QTime>
#include <thread>
#include "mainwindow.h"


namespace Ui {
class Success;
}

class Success : public QDialog
{
    Q_OBJECT

public:
    explicit Success(QWidget *parent = 0);
    ~Success();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Success *ui;
    bool running;
    QTime currentTime;
    std::thread *clockThread;

    static void timeUpgrade(QTime* currentTime, Ui::Success *ui, bool* running);

};

#endif // SUCCESS_H
