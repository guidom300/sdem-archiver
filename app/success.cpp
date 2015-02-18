#include "success.h"
#include "ui_success.h"
#include <QLCDNumber>

Success::Success(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Success)
{
    ui->setupUi(this);
    ui->pushButton->setDisabled(true);
   // ui->labelSuccess->setText("File compressed successfully!");
    ui->lcd->display(0);
    running = false;
    clockThread = new std::thread(timeUpgrade, &currentTime, ui, &running);
    encoderThread = new std::thread(start, input, output, nThreads, dict);

}

Success::~Success()
{
    delete clockThread;
    delete ui;
}

void Success::on_pushButton_clicked()
{
    running = false;
    this->close();
}

void Success::timeUpgrade(QTime* currentTime, Ui::Success *ui, bool* running)
{
    currentTime->start();
    while(*running)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        ui->lcd->display(currentTime->elapsed());
    }
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

