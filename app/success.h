#ifndef SUCCESS_H
#define SUCCESS_H

#include <QDialog>
#include "mainwindow.h"


namespace Ui {
class Success;
}

class Success : public QDialog
{
    Q_OBJECT

public:
    explicit Success(qint32 &time, QWidget *parent = 0);
    ~Success();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Success *ui;
    qint32 m_time;
};

#endif // SUCCESS_H
