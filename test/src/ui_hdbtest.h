/********************************************************************************
** Form generated from reading UI file 'hdbtest.ui'
**
** Created: Tue May 7 13:53:24 2013
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HDBTEST_H
#define UI_HDBTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "EPlotLight"
#include "EPlotLightMarker"
#include "TPlotLightMarker"
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_HdbTest
{
public:
    QVBoxLayout *verticalLayout;
    TPlotLightMarker *plot;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *pbGetSources;
    QPushButton *pbClearPlot;
    QSplitter *splitter;
    QTreeWidget *twArchived;
    QTreeWidget *twIntervals;
    QLabel *label;
    QDateTimeEdit *dteStart;
    QLabel *label_3;
    QDateTimeEdit *dteStop;
    QPushButton *pbClearCurve;
    QPushButton *pbSetSource;
    QCheckBox *cbLive;

    void setupUi(QWidget *HdbTest)
    {
        if (HdbTest->objectName().isEmpty())
            HdbTest->setObjectName(QString::fromUtf8("HdbTest"));
        HdbTest->resize(704, 862);
        verticalLayout = new QVBoxLayout(HdbTest);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        plot = new TPlotLightMarker(HdbTest);
        plot->setObjectName(QString::fromUtf8("plot"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(plot);

        groupBox = new QGroupBox(HdbTest);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pbGetSources = new QPushButton(groupBox);
        pbGetSources->setObjectName(QString::fromUtf8("pbGetSources"));

        gridLayout->addWidget(pbGetSources, 0, 0, 1, 2);

        pbClearPlot = new QPushButton(groupBox);
        pbClearPlot->setObjectName(QString::fromUtf8("pbClearPlot"));

        gridLayout->addWidget(pbClearPlot, 0, 2, 1, 1);

        splitter = new QSplitter(groupBox);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        twArchived = new QTreeWidget(splitter);
        twArchived->setObjectName(QString::fromUtf8("twArchived"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(5);
        sizePolicy1.setVerticalStretch(2);
        sizePolicy1.setHeightForWidth(twArchived->sizePolicy().hasHeightForWidth());
        twArchived->setSizePolicy(sizePolicy1);
        twArchived->setSelectionMode(QAbstractItemView::ExtendedSelection);
        twArchived->setRootIsDecorated(false);
        splitter->addWidget(twArchived);
        twIntervals = new QTreeWidget(splitter);
        twIntervals->setObjectName(QString::fromUtf8("twIntervals"));
        twIntervals->setSelectionMode(QAbstractItemView::ExtendedSelection);
        splitter->addWidget(twIntervals);

        gridLayout->addWidget(splitter, 1, 0, 1, 6);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        dteStart = new QDateTimeEdit(groupBox);
        dteStart->setObjectName(QString::fromUtf8("dteStart"));
        dteStart->setCalendarPopup(true);

        gridLayout->addWidget(dteStart, 2, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 2, 1, 1);

        dteStop = new QDateTimeEdit(groupBox);
        dteStop->setObjectName(QString::fromUtf8("dteStop"));
        dteStop->setCalendarPopup(true);

        gridLayout->addWidget(dteStop, 2, 3, 1, 1);

        pbClearCurve = new QPushButton(groupBox);
        pbClearCurve->setObjectName(QString::fromUtf8("pbClearCurve"));

        gridLayout->addWidget(pbClearCurve, 0, 3, 1, 1);

        pbSetSource = new QPushButton(groupBox);
        pbSetSource->setObjectName(QString::fromUtf8("pbSetSource"));

        gridLayout->addWidget(pbSetSource, 2, 4, 1, 1);

        cbLive = new QCheckBox(groupBox);
        cbLive->setObjectName(QString::fromUtf8("cbLive"));

        gridLayout->addWidget(cbLive, 2, 5, 1, 1);


        verticalLayout->addWidget(groupBox);


        retranslateUi(HdbTest);

        QMetaObject::connectSlotsByName(HdbTest);
    } // setupUi

    void retranslateUi(QWidget *HdbTest)
    {
        HdbTest->setWindowTitle(QApplication::translate("HdbTest", "HdbTest", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("HdbTest", "Configuration", 0, QApplication::UnicodeUTF8));
        pbGetSources->setText(QApplication::translate("HdbTest", "Get Archived Sources", 0, QApplication::UnicodeUTF8));
        pbClearPlot->setText(QApplication::translate("HdbTest", "Clear Plot", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = twArchived->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("HdbTest", "sources", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = twIntervals->headerItem();
        ___qtreewidgetitem1->setText(4, QApplication::translate("HdbTest", "per per mod", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(3, QApplication::translate("HdbTest", "archiver", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(2, QApplication::translate("HdbTest", "New Column", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(1, QApplication::translate("HdbTest", "stop date", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("HdbTest", "start date", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HdbTest", "from:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("HdbTest", "to:", 0, QApplication::UnicodeUTF8));
        pbClearCurve->setText(QApplication::translate("HdbTest", "Clear Selected Curve", 0, QApplication::UnicodeUTF8));
        pbSetSource->setText(QApplication::translate("HdbTest", "set source", 0, QApplication::UnicodeUTF8));
        cbLive->setText(QApplication::translate("HdbTest", "Live", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HdbTest: public Ui_HdbTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HDBTEST_H
