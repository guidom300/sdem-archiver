#include "success.h"
#include "ui_success.h"
#include <QLCDNumber>

Success::Success(qint32 &time, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Success),
    m_time(time)
{
    ui->setupUi(this);
    ui->labelSuccess->setText("File compressed successfully!");
    ui->lcd->display(m_time);
}

Success::~Success()
{
    delete ui;
}

void Success::on_pushButton_clicked()
{
    this->close();
   /* Mainwindow ;
    success.setModal(true);
    success.exec();
    */
}
