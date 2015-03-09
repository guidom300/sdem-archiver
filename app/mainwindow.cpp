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

    ui->tabWidget->setTabText(0, "Encoder");
    ui->tabWidget->setTabText(1, "Decoder");

    //encoder
    ui->bm->setChecked(true);
    ui->spinBox->setValue(4);   //default
    ui->pushButton_3->setEnabled(false);
    ui->destinationEncoder->setText(QDir::homePath());

    //decoder
    ui->dec_startButton->setEnabled(false);
    ui->dec_threads->setValue(4);
    ui->destinationDecoder->setText(QDir::homePath());

    setAcceptDrops(true);

    this->adjustSize();
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
    ui->destinationEncoder->setText(dir);
}

void MainWindow::on_pushButton_2_clicked()
{
    //Choose file button
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose input file"), QDir::homePath(), "All Files (*.*)");
    ui->inputFileEncoder->setText(filename);
}

void MainWindow::on_pushButton_3_clicked()
{
    //Start button
    qint32 nThreads = ui->spinBox ->value();
    bool dictType = ui->bm->isChecked();

    QString filepath = ui->inputFileEncoder->text();
    QString directory = ui->destinationEncoder->text();
    QFileInfo fi(filepath);
    QString base = fi.baseName();
    QString suffix = fi.suffix();
    QString outputName = QDir(directory).filePath(base+"."+suffix+".enc");
    QFileInfo output( outputName );

    if(!fi.exists())
    {
        QMessageBox::critical(this, "Error", filepath + " does not exist.",
                              QMessageBox::Ok);
    }
    else if(fi.size() == 0)
    {
        QMessageBox::critical(this, "Error", filepath + " is empty. Choose another input file.",
                              QMessageBox::Ok);
    }
    else
    {
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

        EncoderProgressDialog dialog(filepath, outputName, nThreads, dictType);
        dialog.setModal(true);
        dialog.exec();
    }
}


void MainWindow::on_inputFileEncoder_textChanged()
{
    bool enabled = false;
    QFileInfo inputFile(ui->inputFileEncoder->text());
    QFileInfo destination(ui->destinationEncoder->text());

    if(inputFile.isFile() && destination.isDir()) {
        enabled = true;
    }

    ui->pushButton_3->setEnabled(enabled);
}

void MainWindow::on_destinationEncoder_textChanged()
{
    on_inputFileEncoder_textChanged();
}
//fine encoder

//decoder
void MainWindow::on_inputFileDecoder_textChanged()
{
    bool enabled = false;
    QFileInfo inputFile(ui->inputFileDecoder->text());
    QFileInfo destination(ui->destinationDecoder->text());

    if(inputFile.isFile() && destination.isDir()) {
        enabled = inputFile.suffix() == "enc";
    }

    ui->dec_startButton->setEnabled(enabled);
 }

void MainWindow::on_destinationDecoder_textChanged()
{
    on_inputFileDecoder_textChanged();
}

void MainWindow::on_dec_inputButton_clicked()
{
    //Choose file button
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file to decode"), QDir::homePath(), "Enc Files(*.enc)");
    ui->inputFileDecoder->setText(filename);
}

void MainWindow::on_dec_destButton_clicked()
{
    //Destination folder button
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose destination folder"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->destinationDecoder->setText(dir);
}

void MainWindow::on_dec_startButton_clicked()
{
    //Start decoder button
    qint32 nThreads = ui->dec_threads->value();

    QString filepath = ui->inputFileDecoder->text();
    QString directory = ui->destinationDecoder->text();
    QFileInfo file(filepath);
    QString base = file.baseName();
    QString fileInput = filepath;
    fileInput = fileInput.mid(0, fileInput.length()-4);
    QFileInfo fi(fileInput);
    QString suffix = fi.suffix();
    QString outputName = QDir(directory).filePath(base+"."+suffix);
    QFileInfo output( outputName );

    if(!fi.exists())
    {
        QMessageBox::critical(this, "Error", filepath + " does not exist.",
                              QMessageBox::Ok);
    }
    else if(fi.size() == 0)
    {
        QMessageBox::critical(this, "Error", filepath + " is empty. Choose another input file.",
                              QMessageBox::Ok);
    }
    else
    {
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

        DecoderProgressDialog dialog(filepath, outputName, nThreads);
        dialog.setModal(true);
        dialog.exec();
    }
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

    ui->inputFileEncoder->setText(fileName);
}
