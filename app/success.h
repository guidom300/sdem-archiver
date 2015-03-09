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
    explicit Success(QString input, QString output, int nThreads, bool dictType, QWidget *parent = 0);
    ~Success();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Success *ui;
    bool running;
    QTime currentTime;
    std::thread *clockThread;
    std::thread *startEncoder;

    static void timeUpgrade(Success* dialog, QString input, QString output);
    static void start(QString input, QString output, int nThreads, bool dictType, bool* running);

};

#endif // SUCCESS_H
