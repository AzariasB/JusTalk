
TEMPLATE = app

DEPENDPATH += .
TARGET = client
INCLUDEPATH += .

CONFIG += c++11


QT += designer uitools core gui network


RESOURCES += \
    icons.qrc

FORMS   += \ 
    justalkclient.ui

HEADERS += \ 
    justalkclient.h \
    ../actionlist.h \
    infodialog.h

SOURCES += \
           main.cpp \
    justalkclient.cpp \
    ../actionlist.cpp \
    infodialog.cpp

