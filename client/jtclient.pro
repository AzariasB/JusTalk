
TEMPLATE = app

DEPENDPATH += .
TARGET = client
INCLUDEPATH += .

QT += designer uitools core gui network


RESOURCES += images.qrc

FORMS   += MainWindow.ui 

HEADERS += MainWindow.h 

SOURCES += MainWindow.cpp \
           main.cpp

