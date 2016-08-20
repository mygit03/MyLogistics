#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T09:15:38
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyLogistics
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

DISTFILES += \
    icon.rc

RC_FILE += \
    icon.rc
