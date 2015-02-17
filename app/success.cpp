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
    while(&running)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        ui->lcd->display(currentTime->elapsed());
    }
}
