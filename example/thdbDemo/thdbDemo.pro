#-------------------------------------------------
#
# Project created by QtCreator 2011-05-24T09:43:58
#
#-------------------------------------------------

BIN_DIR = /runtime/bin

QT       += core gui sql

TARGET = bin/thdbDemo
TEMPLATE = app

examplebin.files = bin/thdbDemo
examplebin.path = $${BIN_DIR}

unix:INCLUDEPATH += ../..

unix:LIBS += -L../../ -lTHdb

UI_DIR = ui
MOC_DIR = moc
OBJECTS_DIR = obj

SOURCES += main.cpp\
        widget.cpp \
    about.cpp


HEADERS  += widget.h \
    about.h

FORMS    += widget.ui

INSTALLS += examplebin
