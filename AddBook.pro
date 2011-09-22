#-------------------------------------------------
#
# Project created by QtCreator 2011-07-20T17:27:27
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += qt3support

TARGET = AddBook
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    searchfilter.cpp \
    contact.cpp

HEADERS  += mainwindow.h \
    searchfilter.h \
    contact.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
