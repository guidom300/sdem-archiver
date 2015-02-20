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
    _input(input),
    _output(output),
    _nThreads(nThreads),
    _dictType(dictType),
    ui(new Ui::Success)
{
    ui->setupUi(this);
    ui->pushButton->setDisabled(true);
    ui->labelSuccess->setText("");
    ui->lcd->display(0);
    running = true;
    startEncoder = new std::thread(start, _input.toStdString().c_str(), _output.toStdString().c_str(), _nThreads, _dictType, &running);
    clockThread = new std::thread(timeUpgrade, &currentTime, ui, &running, _input, _output);
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

void Success::timeUpgrade(QTime* currentTime, Ui::Success *ui, bool* running, QString input, QString output)
{
    currentTime->start();
    while(*running)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        ui->lcd->display(currentTime->elapsed());
    }

    ui->labelSuccess->setText("File compressed successfully!");
    ui->pushButton->setEnabled(true);
    QFileInfo fi(input);
    QFileInfo fo(output);
    float compressionRatio = ((float)fo.size() / (float)fi.size()) * 100;
    ui->lcd_2->display(compressionRatio);
}

void Success::start(const char* input, const char* output, int nThreads, bool dictType, bool* running)
{
    encode_in_parallel(input, output, nThreads);

    *running = false;
}
