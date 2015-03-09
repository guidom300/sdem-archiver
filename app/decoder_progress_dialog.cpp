#include "decoder_progress_dialog.h"
#include "ui_decoder_progress_dialog.h"
#include <QLCDNumber>
#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include <QDebug>
#include "parallel.h"

using namespace std;

DecoderProgressDialog::DecoderProgressDialog(QString input, QString output, int nThreads, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecoderProgressDialog),
    running(true),
    clock(new std::thread(dec_timeUpgrade, this)),
    encoder(new std::thread(dec_start, input, output, nThreads, &running))
{
    ui->setupUi(this);
    ui->dec_pushButton->setDisabled(true);
    ui->dec_lcd->display(0);

    this->adjustSize();
}

DecoderProgressDialog::~DecoderProgressDialog()
{
    encoder->join();
    delete encoder;

    clock->join();
    delete clock;

    delete ui;
}


void DecoderProgressDialog::dec_timeUpgrade(DecoderProgressDialog* dialog)
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

void DecoderProgressDialog::dec_start(QString input, QString output, int nThreads, bool* running)
{
    decode_in_parallel(input.toStdString().c_str(), output.toStdString().c_str(), nThreads);

    *running = false;
}

void DecoderProgressDialog::on_dec_pushButton_clicked()
{
    this->close();
}
