#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    this->setBaseSize(350, 235);
    this->setFixedSize(this->baseSize());

    this->setWindowTitle("About Us");

    this->nucaLabel = new QLabel(this);
    this->nucaLabel->setGeometry(139, 7, 100, 15);
    this->nucaLabel->setText("<a href='http://code.google.com/p/nuca/' style='text-decoration:none;'>NUCA V1.0</>");
    this->nucaLabel->setOpenExternalLinks(true);

    this->logoLabel = new QLabel(this);
    this->logoLabel->setGeometry(60, 13, 220, 150);
    this->logoLabel->setText("<a href='http://fudepan.org.ar/'><img src='img/logo-fudepan.png' /></>");
    this->logoLabel->setToolTip(tr("Fundacion para el desarrollo de la Programacion en acidos Nucleicos"));
    this->logoLabel->setOpenExternalLinks(true);

    this->contributorsLabel = new QLabel(this);
    this->contributorsLabel->setGeometry(105, 120, 150, 100);
    this->contributorsLabel->setText("<P align='center'>Juan Cruz<BR>Daniel Gutson<BR>Leonardo Boquillon</P>");

    this->okButton = new QPushButton(this);
    this->okButton->setGeometry(160, 205, 40, 20);
    this->okButton->setText("OK");

    connect(this->okButton, SIGNAL(clicked()), this, SLOT(okButtonAction()));
}

AboutDialog::~AboutDialog()
{
    delete this->logoLabel;
    delete this->contributorsLabel;
    delete this->nucaLabel;
    delete this->okButton;
}

void AboutDialog::okButtonAction()
{
    this->close();
}
