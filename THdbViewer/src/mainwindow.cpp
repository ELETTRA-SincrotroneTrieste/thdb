#include <THdbSingleton>
#include "mainwindow.h"
#include "thdbplot.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* register view widgets */
    /* Plot */
    THdbPlot *thdbPlot = new THdbPlot(this);
    thdbPlot->setTHdb(THdbSingleton::instance());
    thdbPlot->setType(ui->configWidget->hdb() ? THdbBase::Historical : THdbBase::Temporary);
    connect(ui->configWidget, SIGNAL(viewClicked()), this, SLOT(slotViewClicked()));
    connect(ui->configWidget, SIGNAL(sourceRemoved(const QString&)),
            thdbPlot, SLOT(removeHistoricalCurve(const QString&)));
    connect(ui->configWidget, SIGNAL(liveEnabled(bool)),
            thdbPlot, SLOT(setLiveModeEnabled(bool)));
    connect(ui->configWidget, SIGNAL(hdbSelected(bool)), this, SLOT(slotHdbSelected(bool)));
    connect(thdbPlot, SIGNAL(historicalDataUpdated()), this, SLOT(slotPlotHistoricalDataUpdated()));
    /* register plot */
    ui->historicalViewWidget->registerWidget(thdbPlot, HistoricalViewWidget::NumberScalar);

    connect(ui->pbConfigure, SIGNAL(clicked()), this, SLOT(slotConfigureClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotPlotHistoricalDataUpdated()
{
    static_cast<THdbPlot *>(sender())->update(ui->configWidget->sources());
}

void MainWindow::slotHdbSelected(bool hdb)
{
    THdbSingleton *thdb = THdbSingleton::instance();
    if(hdb && thdb->type() != THdbBase::Historical)
        thdb->setType(THdbBase::Historical, HdbConnectionParams());
    else if(!hdb && thdb->type() != THdbBase::Temporary)
        thdb->setType(THdbBase::Temporary, TdbConnectionParams());
}

void MainWindow::slotConfigureClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::slotViewClicked()
{
    /* retrieve singleton instance */
    THdbSingleton *thdb = THdbSingleton::instance();
    QDateTime startDt = ui->configWidget->startDateTime();
    QDateTime stopDt = ui->configWidget->stopDateTime();
    QStringList sources = ui->configWidget->sources();
    foreach(QString source, sources)
        thdb->getData(source, startDt, stopDt);

    /* switch stacked widget index */
    ui->stackedWidget->setCurrentIndex(0);
}
