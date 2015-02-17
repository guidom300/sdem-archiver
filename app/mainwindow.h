#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "success.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_textBrowser_2_textChanged();

    void on_textBrowser_textChanged();

    void on_dec_inputPath_textChanged();

    void on_dec_destPath_textChanged();

    void on_dec_inputButton_clicked();

    void on_dec_destButton_clicked();

    void on_dec_startButton_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
