#ifndef DEC_SUCCESS_H
#define DEC_SUCCESS_H

#include <QDialog>
#include <QTime>
#include <thread>
#include "mainwindow.h"

namespace Ui {
class Dec_success;
}

class Dec_success : public QDialog
{
    Q_OBJECT

public:
    explicit Dec_success(QString input, QString output, int nThreads, QWidget *parent = 0);
    ~Dec_success();

private slots:
    void on_dec_pushButton_clicked();

private:
    Ui::Dec_success *ui;
    bool running;
    QTime currentTime;
    std::thread *clock;
    std::thread *encoder;

    static void dec_timeUpgrade(Dec_success* dialog);
    static void dec_start(QString input, QString output, int nThreads, bool* running);
};

#endif // DEC_SUCCESS_H
