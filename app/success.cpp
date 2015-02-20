#include "success.h"
#include "ui_success.h"
#include <QLCDNumber>
#include <iostream>
#include <fstream>
#include <ios>
#include <iterator>
#include "splitter.h"
#include "worker.h"
#include "encoder_wrapper.h"
#include "lzss_encoder.h"
#include "huffman_encoder_stack.h"
#include <QDebug>

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
    running = false;
    startEncoder = new std::thread(start, (const char*) _input.toLocal8Bit().data(), (const char*) _output.toLocal8Bit().data(), _nThreads, _dictType, &running);
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
    *running = true;

    ifstream input_file(input, ios::binary);
      ofstream output_file(output, ios::binary | ios::trunc);

      input_file >> noskipws;

      Splitter<Worker<EncoderWrapper<LZSSEncoder<12, 4>>>,
               HuffmanEncoderStack<char>> splitter;

      splitter(std::istreambuf_iterator<char>(input_file.rdbuf()),
               std::istreambuf_iterator<char>(),
               std::ostreambuf_iterator<char>(output_file),
               nThreads,
               32 * 1024);

      *running = false;

}

