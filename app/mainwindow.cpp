#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QUrl>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowIcon(QIcon("/Users/gm/Download/Fire-Toy-icon.png"));

    ui->tabWidget->setTabText(0, "Encoder");
    ui->tabWidget->setTabText(1, "Decoder");


    //encoder
    ui->naive->setChecked(true);
    ui->spinBox->setValue(4);   //default
    ui->pushButton_3->setEnabled(false);
    /*
    QMovie *movie = new QMovie("/Users/gm/Download/loading.gif");
    QLabel *processLabel = new QLabel(this);
    processLabel->setMovie(movie);
    //ui -> label->setMovie(movie);
    movie->start();
    */

    //decoder
    ui->dec_startButton->setEnabled(false);
    ui->dec_threads->setValue(4);

    setAcceptDrops(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    //Destination folder button
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose destination folder"),
                                                QDir::homePath(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    ui->textBrowser->setText(dir);
}

void MainWindow::on_pushButton_2_clicked()
{
    //Choose file button
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose input file"), QDir::homePath(), "All files (*.*); ; Text Files(*.txt)");
    ui->textBrowser_2->setText(filename);
}

void MainWindow::on_pushButton_3_clicked()
{
    //Start button
    qint32 nThreads = ui->spinBox ->value();
    bool dictType = false; //default naive
    if(ui->bm->isChecked())
        {
            dictType = true;
        }

    //invoco funzione di compressione passando i parametri dict (0 = naive, 1 = bm) e threads
    // all'interno, al temrine della funzione di compressione

    //this->hide();


    QString filepath = ui->textBrowser_2->toPlainText();
    QString directory = ui->textBrowser->toPlainText();
    QFileInfo fi(filepath);
    QString base = fi.baseName();
    QString outputName = QDir(directory).filePath(base+".enc");
    QFileInfo output( outputName );
    if(output.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Attention", outputName + " already exists, do you want to overwrite it?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }

    }
    Success success(filepath, outputName, nThreads, dictType);
    success.setModal(true);
    success.exec();

}


void MainWindow::on_textBrowser_2_textChanged()
{
    if(!(ui->textBrowser_2->toPlainText().trimmed().isEmpty()))
        if(!(ui->textBrowser->toPlainText().trimmed().isEmpty()))
            ui->pushButton_3->setEnabled(true);
}

void MainWindow::on_textBrowser_textChanged()
{
    if(!(ui->textBrowser->toPlainText().trimmed().isEmpty()))
        if(!(ui->textBrowser_2->toPlainText().trimmed().isEmpty()))
            ui->pushButton_3->setEnabled(true);
}
//fine encoder

//decoder
void MainWindow::on_dec_inputPath_textChanged()
{
    if(!(ui->dec_inputPath->toPlainText().trimmed().isEmpty()))
        if(!(ui->dec_destPath->toPlainText().trimmed().isEmpty()))
            ui->dec_startButton->setEnabled(true);

}

void MainWindow::on_dec_destPath_textChanged()
{
    if(!(ui->dec_destPath->toPlainText().trimmed().isEmpty()))
        if(!(ui->dec_inputPath->toPlainText().trimmed().isEmpty()))
            ui->dec_startButton->setEnabled(true);
}

void MainWindow::on_dec_inputButton_clicked()
{
    //Choose file button
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file to decode"), "/Users/gm/Documents/", "All files (*.*); ; Text Files(*.txt)");
    ui->dec_inputPath->setText(filename);
}

void MainWindow::on_dec_destButton_clicked()
{
    //Destination folder button
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose destination folder"),
                                                "/Users/gm/Documents/",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    ui->dec_destPath->setText(dir);
}

void MainWindow::on_dec_startButton_clicked()
{
    //Start decoder button
    qint32 threads = ui->dec_threads->value();

    //invoco funzione di compressione passando i parametri dict (0 = naive, 1 = bm) e threads
    // all'interno, al temrine della funzione di compressione

    //this->hide();
    Success success;
    success.setModal(true);
    success.exec();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    //https://developer.apple.com/library/mac/documentation/Cocoa/Reference/Foundation/Classes/NSURL_Class/
    /*
    if (url.host().isEmpty() && url.path().startsWith(QLatin1String("/.file/id=")))
        url = QUrl::fromNSURL([url.toNSURL() filePathURL]);

   url.setPath(url.path().normalized(QString::NormalizationForm_C));
   ret.append(url);
    */
    if (fileName.isEmpty())
        return;

    ui->textBrowser_2->setText(fileName);
}
