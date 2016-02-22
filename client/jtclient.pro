
TEMPLATE = app

DEPENDPATH += .
TARGET = client
INCLUDEPATH += .

QT += designer uitools core gui network


RESOURCES +=

FORMS   += \ 
    justalkclient.ui

HEADERS += \ 
    justalkclient.h

SOURCES += \
           main.cpp \
    justalkclient.cpp

