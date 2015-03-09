#include "success.h"
#include "ui_success.h"
#include <QLCDNumber>
#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include <QDebug>
#include "parallel.h"

using namespace std;

Success::Success(QString input, QString output, int nThreads, bool dictType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Success),
    running(true),
    clockThread(new std::thread(timeUpgrade, this, input, output)),
    startEncoder(new std::thread(start, input, output, nThreads, dictType, &running))
{
    ui->setupUi(this);
    ui->pushButton->setDisabled(true);
    ui->lcd->display(0);

    this->adjustSize();
}

Success::~Success()
{
    startEncoder->join();
    delete startEncoder;

    clockThread->join();
    delete clockThread;

    delete ui;
}

void Success::on_pushButton_clicked()
{
    this->close();
}

void Success::timeUpgrade(Success* dialog, QString input, QString output)
{
    dialog->currentTime.start();
    while(dialog->running)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        dialog->ui->lcd->display(dialog->currentTime.elapsed());
    }

    dialog->setWindowTitle("File compressed successfully!");
    dialog->ui->pushButton->setEnabled(true);
    QFileInfo fi(input);
    QFileInfo fo(output);
    float compressionRatio = ((float)fo.size() / (float)fi.size()) * 100;
    dialog->ui->lcd_2->display(compressionRatio);
}

void Success::start(QString input, QString output, int nThreads, bool dictType, bool* running)
{
    encode_in_parallel(input.toStdString().c_str(), output.toStdString().c_str(), nThreads, dictType);

    *running = false;
}
