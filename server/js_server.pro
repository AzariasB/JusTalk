TEMPLATE = app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

QT += designer core gui network

HEADERS += \
    justalkserver.h
SOURCES += main.cpp \
    justalkserver.cpp
