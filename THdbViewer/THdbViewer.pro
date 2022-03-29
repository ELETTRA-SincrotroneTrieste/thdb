#-------------------------------------------------
#
# Project created by QtCreator 2011-06-01T15:01:49
#
#-------------------------------------------------

include(/runtime/include/qtango4/qtango.pri)

unix:INCLUDEPATH += .. ../widgets ../ui

QT       += core gui

UI_DIR = ui
OBJECTS_DIR = obj

TARGET = bin/THdbViewer
TEMPLATE = app



SOURCES += src/main.cpp\
        src/mainwindow.cpp \
#    src/plotadapter/tdbplotadapter.cpp \
#    src/plotadapter/hdbplotadapter.cpp \
#    src/plotadapter/dbplotbackground.cpp \
#    src/plotadapter/dbplotadapterinterfaceprivate.cpp \
#    src/plotadapter/dbplotadapterinterface.cpp \
    src/thdbplot.cpp \
    src/thdbplotprivate.cpp

HEADERS  += src/mainwindow.h \
#    src/plotadapter/tdbplotadapter.h \
#    src/plotadapter/hdbplotadapter.h \
#    src/plotadapter/dbplotbackground.h \
#    src/plotadapter/dbplotadapterinterfaceprivate.h \
#    src/plotadapter/dbplotadapterinterface.h \
    src/thdbplot.h \
    src/thdbplotprivate.h

FORMS    += src/mainwindow.ui

unix:LIBS += -lTHdb
