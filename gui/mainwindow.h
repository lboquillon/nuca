#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include "service.h"

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
    void on_about_triggered();

private:
    Ui::MainWindow* ui;
};

#endif
