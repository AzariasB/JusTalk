
TEMPLATE = app

DEPENDPATH += .
TARGET = server
INCLUDEPATH += .


QT += designer uitools core gui network


RESOURCES +=

FORMS   += \
    server.ui

HEADERS += \
    justalkserver.h

SOURCES += \
           main.cpp \
    justalkserver.cpp

