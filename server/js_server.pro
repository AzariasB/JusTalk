TEMPLATE = app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += console

QT += core gui network

HEADERS += \
    justalkserver.h
SOURCES += main.cpp \
    justalk.cpp
