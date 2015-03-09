#ifndef DEC_SUCCESS_H
#define DEC_SUCCESS_H

#include <QDialog>
#include <QTime>
#include <thread>
#include "mainwindow.h"

namespace Ui {
class DecoderProgressDialog;
}

class DecoderProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecoderProgressDialog(QString input, QString output, int nThreads, QWidget *parent = 0);
    ~DecoderProgressDialog();

private slots:
    void on_dec_pushButton_clicked();

private:
    Ui::DecoderProgressDialog *ui;
    bool running;
    QTime currentTime;
    std::thread *clock;
    std::thread *encoder;

    static void dec_timeUpgrade(DecoderProgressDialog* dialog);
    static void dec_start(QString input, QString output, int nThreads, bool* running);
};

#endif // DEC_SUCCESS_H
