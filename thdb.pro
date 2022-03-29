# $Id: thdb.pro,v 1.13 2012-10-11 08:13:11 giacomo Exp $
# $Name $

QT += widgets

INSTALL_ROOT = /usr/local/thdb

SHAREDIR = $${INSTALL_ROOT}/share
INC_DIR = $${INSTALL_ROOT}/include/thdb
LIB_DIR = $${INSTALL_ROOT}/lib
BIN_DIR = $${INSTALL_ROOT}/bin

lib.path = $${LIB_DIR}

VERSION = 1.0.5
VER_MAJ = 1
VER_MIN = 0
VER_FIX = 5

DEFINES += THDB_VERSION=\"\\\"$${VERSION}\\\"\" \
    VER_MAJ=$${VER_MAJ} \
    VER_MIN=$${VER_MIN} \
    VER_FIX=$${VER_FIX}

TEMPLATE = lib staticlib

CONFIG += debug
CONFIG -= warn_off silent

FORMS += widgets/configWidget.ui
UI_DIR = ui
OBJECTS_DIR = obj
MOC_DIR = moc

QT += sql

TARGET = THdb

QMAKE_CLEAN += libTHdb*

DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QTANGO_PRINTINFO

DEPENDPATH += . \
              private

INCLUDEPATH += . \
               private \
	       widgets \
               plotadapter \
               queries \
              private/queries \
                widgets/private \
                ui

# Input
HEADERS += thdbbase.h \
        thdb.h \
        private/thdbbaseprivate.h \
    dbproxy.h \
    private/dbqueryevent.h \
    dbqueryinterface.h \
    private/queryresultevent.h \
    private/queries/sourceslistquery.h \
    private/queries/scalarnumbersquery.h \
    private/queries/attidquery.h \
    private/queries/idinfo.h \
    private/queries/datainfo.h \
    private/queries/querydatainfo.h \
    private/queries/aptinfo.h \
    private/queries/aptquery.h \
    queries/aptdata.h \
    private/queries/amtquery.h \
    private/queries/amtinfo.h \
    queries/amtdata.h \
    thdbsingleton.h \
    widgets/thdbconfigwidget.h \
    dbconnectionparams.h \
    private/switchdatabaseevent.h \
    widgets/archivedsourcestreewidget.h \
    widgets/sourcestreewidget.h \
    widgets/private/sourcestreewidget_p.h \
    widgets/historicalviewwidget.h \
    widgets/private/historicalviewwidgetprivate.h

SOURCES += thdbbase.cpp \
        thdb.cpp \
        private/thdbbaseprivate.cpp \
    dbproxy.cpp \
    private/dbqueryevent.cpp \
    dbqueryinterface.cpp \
    private/queryresultevent.cpp \
    private/queries/sourceslistquery.cpp \
    private/queries/scalarnumbersquery.cpp \
    private/queries/attidquery.cpp \
    private/queries/idinfo.cpp \
    private/queries/datainfo.cpp \
    private/queries/querydatainfo.cpp \
    private/queries/aptinfo.cpp \
    private/queries/aptquery.cpp \
    queries/aptdata.cpp \
    private/queries/amtquery.cpp \
    private/queries/amtinfo.cpp \
    queries/amtdata.cpp \
    thdbsingleton.cpp \
    dbconnectionparams.cpp \
    private/switchdatabaseevent.cpp \
    widgets/archivedsourcestreewidget.cpp \
    widgets/sourcestreewidget.cpp \
    widgets/private/sourcestreewidget_p.cpp \
    widgets/historicalviewwidget.cpp \
    widgets/private/historicalviewwidgetprivate.cpp \
    widgets/thdbconfigwidget.cpp

# source documentation. Mainpage has been inserted into tapplicationspoolproxy.h

DOC_DIR = $${SHAREDIR}/thdb
doc.commands = \
# doxytag \
#    -t \
#    qt.tag \
 #  /usr/share/qt4/doc/html/; \
    doxygen \
    Doxyfile;
doc.files = doc/
doc.path = $${DOC_DIR}

# lib
lib.path = $${LIB_DIR}
lib.files = libTHdb.so.$${VERSION}
lib.commands = ln \
    -sf \
    libTHdb.so.$${VERSION} \
    $${LIB_DIR}/libTHdb.so.$${VER_MAJ} \
    && \
    ln \
    -sf \
    libTHdb.so.$${VER_MAJ} \
    $${LIB_DIR}/libTHdb.so

inc.files = \
    THdb  APTInfo queries/APTData queries/AMTData \
    HdbProxy DbProxy DbQueryInterface DbConnectionParams \
    THdbBase \
    THdbSingleton \
    widgets/ArchivedSourcesTreeWidget \
    widgets/HistoricalViewWidget \
    widgets/SourcesTreeWidget \
    widgets/THdbConfigWidget \
    ui/ui_configWidget.h

inc.files += $${HEADERS}

inc.path = $${INC_DIR}


# installation

INSTALLS += lib \
    inc \
    doc
