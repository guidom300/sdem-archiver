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
    std::thread *startEncoder;
    QString input;
    QString output;
    QInt nThreads;
    bool dictType;

    static void timeUpgrade(QTime* currentTime, Ui::Success *ui, bool* running);
    static void start(const char* input, const char* output, int nThreads, bool dictType);

};

#endif // SUCCESS_H
