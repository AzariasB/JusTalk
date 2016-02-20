#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T16:00:57
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JusTalk
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
        ../connection.cpp

HEADERS  += client.h \
        ../connection.h

FORMS    += client.ui
