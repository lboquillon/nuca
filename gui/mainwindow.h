#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QDir>


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on_btnExploreCom_clicked();
    void on_compressButton_clicked();
    void on_btnExploreDec_clicked();
    void on_decompressButton_clicked();

    void on_exit_triggered();

private:
    Ui::MainWindow* ui;

};

#endif
