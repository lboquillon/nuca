#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget* parent = NULL);
    ~AboutDialog();

private:
    QLabel* logoLabel;
    QLabel* nucaLabel;
    QLabel* contributorsLabel;
    QLabel* contributors2Label;
    QPushButton* okButton;

private slots:
    void okButtonAction();
};

#endif // ABOUTDIALOG_H
