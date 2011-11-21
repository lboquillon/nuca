#-------------------------------------------------
#
# Project created by QtCreator 2011-09-14T08:06:14
#
#-------------------------------------------------

QT       += core gui

TARGET = nuca
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    service.cpp \
    application.cpp

HEADERS  += mainwindow.h \
    application.h \
    service.h

FORMS    += mainwindow.ui

INCLUDEPATH += /home/leo/fudepan/fudepan-build/projects/nuca/nuca

DEFINES += MILI_NAMESPACE

LIBS += -lbiopp











