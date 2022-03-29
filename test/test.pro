#-------------------------------------------------
#
# Project created by QtCreator 2011-03-09T10:47:20
#
#-------------------------------------------------

include(/runtime/include/qtango4/qtango.pri)

unix:INCLUDEPATH += /runtime/include/thdb

QT       += core gui

TARGET = test
TEMPLATE = app

DEFINES -= QT_NO_DEBUG_OUTPUT


CONFIG += debug

CONFIG -= silent

SOURCES += main.cpp\
        hdbtest.cpp \
    hdbplot.cpp

HEADERS  += hdbtest.h \
    hdbplot.h

FORMS    += hdbtest.ui

unix:LIBS += -lTHdb -lQTango -L/runtime/lib
