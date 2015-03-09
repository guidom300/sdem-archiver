#include "dec_success.h"
#include "ui_dec_success.h"
#include <QLCDNumber>
#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include <QDebug>
#include "parallel.h"

using namespace std;

Dec_success::Dec_success(QString input, QString output, int nThreads, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dec_success),
    running(true),
    clock(new std::thread(dec_timeUpgrade, this)),
    encoder(new std::thread(dec_start, input, output, nThreads, &running))
{
    ui->setupUi(this);
    ui->dec_pushButton->setDisabled(true);
    ui->dec_lcd->display(0);

    this->adjustSize();
}

Dec_success::~Dec_success()
{
    encoder->join();
    delete encoder;

    clock->join();
    delete clock;

    delete ui;
}


void Dec_success::dec_timeUpgrade(Dec_success* dialog)
{
    dialog->currentTime.start();
    while(dialog->running)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        dialog->ui->dec_lcd->display(dialog->currentTime.elapsed());
    }

    dialog->setWindowTitle("File decoded successfully!");
    dialog->ui->dec_pushButton->setEnabled(true);
}

void Dec_success::dec_start(QString input, QString output, int nThreads, bool* running)
{
    decode_in_parallel(input.toStdString().c_str(), output.toStdString().c_str(), nThreads);

    *running = false;
}

void Dec_success::on_dec_pushButton_clicked()
{
    this->close();
}
