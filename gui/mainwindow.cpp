#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnExploreCom_clicked()
{
    QString file_selected = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::currentPath(), tr("All files (*.*)"));

    if (!file_selected.isNull() && !file_selected.isEmpty())
    {
        ui->compressFileIn->setText(file_selected);
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Information, "NUCA", "Must be select a file");
        msgBox.exec();
    }
}

void MainWindow::on_compressButton_clicked()
{
    if (!ui->compressFileIn->text().isNull()
            && !ui->compressFileOut->text().isNull()
            && !ui->compressFileIn->text().isEmpty()
            && !ui->compressFileOut->text().isEmpty())
    {
        compression(ui->compressFileIn->text().toStdString(), ui->compressFileOut->text().toStdString());

        QMessageBox msgBox(QMessageBox::Information, "NUCA", "Compression Finished");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Information, "NUCA", "Must be select a file");
        msgBox.exec();
    }
}

void MainWindow::on_btnExploreDec_clicked()
{
    QString file_selected = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::currentPath(), tr("All files (*.*)"));

    if (!file_selected.isNull() && !file_selected.isEmpty())
    {
        ui->decompressionFileIn->setText(file_selected);
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Information, "NUCA", "Must be select a file");
        msgBox.exec();
    }
}

void MainWindow::on_decompressButton_clicked()
{
    if (!ui->decompressionFileIn->text().isNull()
            && !ui->decompressionFileOut->text().isNull()
            && !ui->decompressionFileIn->text().isEmpty()
            && !ui->decompressionFileOut->text().isEmpty())
    {
        decompression(ui->decompressionFileIn->text().toStdString(), ui->decompressionFileOut->text().toStdString());

        QMessageBox msgBox(QMessageBox::Information, "NUCA", "Decompression Finished");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Information, "NUCA", "Must be select a file");
        msgBox.exec();
    }

}

void MainWindow::on_exit_triggered()
{
    QCoreApplication::exit();
}
