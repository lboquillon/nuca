#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "mainwindow.h"

class Application : QApplication
{
    Q_OBJECT

public:
    Application(int argc, char** argv)
        : QApplication(argc, argv)
    { }
    int run();

private:
    MainWindow window;

};

#endif
