
TEMPLATE = app

DEPENDPATH += .
TARGET = server
INCLUDEPATH += .

CONFIG += c++11

QT += designer uitools core gui network


RESOURCES +=

FORMS   += \
    server.ui

HEADERS += \
    justalkserver.h

SOURCES += \
           main.cpp \
    justalkserver.cpp

