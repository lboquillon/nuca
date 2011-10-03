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

inline int Application::run()
{
    window.show();
    return this->exec();
}

#endif
