/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Dec 3 16:33:06 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *exit;
    QAction *about;
    QWidget *centralWidget;
    QFrame *frame;
    QTabWidget *tabWidget;
    QWidget *tabCompression;
    QLabel *labelInputCompression;
    QLabel *labelOutputCompression;
    QLineEdit *compressFileIn;
    QLineEdit *compressFileOut;
    QPushButton *btnExploreCom;
    QPushButton *compressButton;
    QWidget *tabDecompression;
    QLineEdit *decompressionFileIn;
    QLineEdit *decompressionFileOut;
    QLabel *labelInputDecompressing;
    QLabel *labelOutputDecompressing;
    QPushButton *btnExploreDec;
    QPushButton *decompressButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(510, 252);
        exit = new QAction(MainWindow);
        exit->setObjectName(QString::fromUtf8("exit"));
        about = new QAction(MainWindow);
        about->setObjectName(QString::fromUtf8("about"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 491, 211));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 471, 191));
        tabCompression = new QWidget();
        tabCompression->setObjectName(QString::fromUtf8("tabCompression"));
        labelInputCompression = new QLabel(tabCompression);
        labelInputCompression->setObjectName(QString::fromUtf8("labelInputCompression"));
        labelInputCompression->setGeometry(QRect(50, 30, 111, 16));
        labelOutputCompression = new QLabel(tabCompression);
        labelOutputCompression->setObjectName(QString::fromUtf8("labelOutputCompression"));
        labelOutputCompression->setGeometry(QRect(10, 80, 161, 16));
        compressFileIn = new QLineEdit(tabCompression);
        compressFileIn->setObjectName(QString::fromUtf8("compressFileIn"));
        compressFileIn->setGeometry(QRect(180, 30, 241, 25));
        compressFileOut = new QLineEdit(tabCompression);
        compressFileOut->setObjectName(QString::fromUtf8("compressFileOut"));
        compressFileOut->setGeometry(QRect(180, 80, 271, 25));
        btnExploreCom = new QPushButton(tabCompression);
        btnExploreCom->setObjectName(QString::fromUtf8("btnExploreCom"));
        btnExploreCom->setGeometry(QRect(420, 30, 31, 25));
        compressButton = new QPushButton(tabCompression);
        compressButton->setObjectName(QString::fromUtf8("compressButton"));
        compressButton->setGeometry(QRect(360, 120, 92, 27));
        tabWidget->addTab(tabCompression, QString());
        tabDecompression = new QWidget();
        tabDecompression->setObjectName(QString::fromUtf8("tabDecompression"));
        decompressionFileIn = new QLineEdit(tabDecompression);
        decompressionFileIn->setObjectName(QString::fromUtf8("decompressionFileIn"));
        decompressionFileIn->setGeometry(QRect(180, 30, 241, 25));
        decompressionFileOut = new QLineEdit(tabDecompression);
        decompressionFileOut->setObjectName(QString::fromUtf8("decompressionFileOut"));
        decompressionFileOut->setGeometry(QRect(180, 80, 271, 25));
        labelInputDecompressing = new QLabel(tabDecompression);
        labelInputDecompressing->setObjectName(QString::fromUtf8("labelInputDecompressing"));
        labelInputDecompressing->setGeometry(QRect(40, 30, 128, 16));
        labelOutputDecompressing = new QLabel(tabDecompression);
        labelOutputDecompressing->setObjectName(QString::fromUtf8("labelOutputDecompressing"));
        labelOutputDecompressing->setGeometry(QRect(7, 80, 168, 15));
        btnExploreDec = new QPushButton(tabDecompression);
        btnExploreDec->setObjectName(QString::fromUtf8("btnExploreDec"));
        btnExploreDec->setGeometry(QRect(420, 30, 31, 25));
        decompressButton = new QPushButton(tabDecompression);
        decompressButton->setObjectName(QString::fromUtf8("decompressButton"));
        decompressButton->setGeometry(QRect(360, 120, 92, 27));
        tabWidget->addTab(tabDecompression, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 510, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(exit);
        menuHelp->addAction(about);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Nuca: Nucleotide Compression Algorithms", 0, QApplication::UnicodeUTF8));
        exit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        about->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        labelInputCompression->setText(QApplication::translate("MainWindow", "File to Compress:", 0, QApplication::UnicodeUTF8));
        labelOutputCompression->setText(QApplication::translate("MainWindow", "File Compressed Name:", 0, QApplication::UnicodeUTF8));
        btnExploreCom->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        compressButton->setText(QApplication::translate("MainWindow", "Compress", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabCompression), QApplication::translate("MainWindow", "Compression", 0, QApplication::UnicodeUTF8));
        labelInputDecompressing->setText(QApplication::translate("MainWindow", "File to Decompress:", 0, QApplication::UnicodeUTF8));
        labelOutputDecompressing->setText(QApplication::translate("MainWindow", "File Decompressed Name:", 0, QApplication::UnicodeUTF8));
        btnExploreDec->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        decompressButton->setText(QApplication::translate("MainWindow", "Decompress", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabDecompression), QApplication::translate("MainWindow", "Decompression", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
