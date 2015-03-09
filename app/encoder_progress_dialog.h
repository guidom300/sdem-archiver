#ifndef SUCCESS_H
#define SUCCESS_H

#include <QDialog>
#include <QTime>
#include <thread>
#include "mainwindow.h"


namespace Ui {
class EncoderProgressDialog;
}

class EncoderProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncoderProgressDialog(QString input, QString output, int nThreads, bool dictType, QWidget *parent = 0);
    ~EncoderProgressDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EncoderProgressDialog *ui;
    bool running;
    QTime currentTime;
    std::thread *clockThread;
    std::thread *startEncoder;

    static void timeUpgrade(EncoderProgressDialog* dialog, QString input, QString output);
    static void start(QString input, QString output, int nThreads, bool dictType, bool* running);

};

#endif // SUCCESS_H
