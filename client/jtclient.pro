
TEMPLATE = app

DEPENDPATH += .
TARGET = client
INCLUDEPATH += .

CONFIG += c++11


QT += designer uitools core gui network


RESOURCES +=

FORMS   += \ 
    justalkclient.ui

HEADERS += \ 
    justalkclient.h \
    ../actionlist.h

SOURCES += \
           main.cpp \
    justalkclient.cpp \
    ../actionlist.cpp

