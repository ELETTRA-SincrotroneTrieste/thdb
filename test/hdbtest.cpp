#include "hdbtest.h"
#include "ui_hdbtest.h"
#include "amtdata.h"
#include <hdbplotadapter.h>
#include <DbConnectionParams>
#include <THdb>
#include <QtDebug>

HdbTest::HdbTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HdbTest)
{
    QThread::currentThread()->setObjectName("hdbTest Main Thread");
    ui->setupUi(this);
    m_thdb = new THdb(this);

    m_thdb->setType(THdbBase::Historical, HdbConnectionParams());

    connect(m_thdb, SIGNAL(archivedSourcesListReady(QStringList)),
            this, SLOT(populateSourcesList(QStringList)));

    connect(ui->pbGetSources, SIGNAL(clicked()), m_thdb, SLOT(getArchivedSources()));

    connect(ui->pbSetSource, SIGNAL(clicked()), this, SLOT(setSource()));

    connect(ui->twArchived, SIGNAL(itemSelectionChanged()), this, SLOT(sourceSelectionChanged()));

    connect(ui->twArchived, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this,
            SLOT(slotItemClicked(QTreeWidgetItem*,int)));

    /* initialize date time edit */
    QDateTime now = QDateTime::currentDateTime();
    ui->dteStop->setDateTime(now);
    ui->dteStart->setDateTime(now.addDays(-1));

    HdbPlotAdapter *hdbPlotAdapter = new HdbPlotAdapter(ui->plot);

//    hdbPlotAdapter->setBackgroundEnabled(true);

    connect(m_thdb,
            SIGNAL(dataAvailable(const QString&, const QVector<double >&, const QVector<double> &)),
            hdbPlotAdapter,
            SLOT(setData(const QString&, const QVector<double >&, const QVector<double> &)));

    connect(m_thdb,
            SIGNAL(dataSetPointAvailable(const QString&, const QVector<double >&, const QVector<double> &)),
            hdbPlotAdapter,
            SLOT(setSetPointData(const QString&, const QVector<double >&, const QVector<double> &)));

    connect(m_thdb, SIGNAL(amtDataReady(const QString&, const AMTData &)),
            this, SLOT(updateAmtData(const QString&, const AMTData&)));

    connect(ui->pbClearPlot, SIGNAL(clicked()), hdbPlotAdapter, SLOT(clearHistoricalData()));
    connect(ui->pbClearCurve, SIGNAL(clicked()), this, SLOT(clearSelectedCurve()));
}

HdbTest::~HdbTest()
{
    delete ui;
}

void HdbTest::setSource()
{
    QStringList newSources;
    QList<QTreeWidgetItem *> items = ui->twArchived->selectedItems();
    foreach(QTreeWidgetItem *it, items)
    {
        m_thdb->getData(it->text(0), ui->dteStart->dateTime(), ui->dteStop->dateTime());
        newSources << it->text(0);
    }
//    HdbPlotAdapter *hdbPlotAdapter = ui->plot->findChild<HdbPlotAdapter *>();
//    hdbPlotAdapter->addBackground(ui->dteStart->dateTime().toTime_t(), ui->dteStop->dateTime().toTime_t());
    QStringList currentSources = ui->plot->sourcesList();
    currentSources += newSources;
    ui->plot->setSourcesList(currentSources);
}

void HdbTest::intervalsSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = ui->twIntervals->selectedItems();
    ui->dteStart->setDisabled(selectedItems.size() > 1);
    ui->dteStop->setDisabled(selectedItems.size() > 1);
    if(selectedItems.size() == 1)
    {

    }
    else if(selectedItems.size() > 1)
    {

    }
}

void HdbTest::clearSelectedCurve()
{
    QList<QTreeWidgetItem *> items = ui->twArchived->selectedItems();
    foreach(QTreeWidgetItem *it, items)
        ui->plot->findChild<HdbPlotAdapter *>()->clearCurveHistory(it->text(0));
    QStringList currentSources = ui->plot->sourcesList();
    foreach(QTreeWidgetItem *it, items)
        currentSources.removeAll(it->text(0));
    ui->plot->setSourcesList(currentSources);
}

void HdbTest::sourceSelectionChanged()
{
    ui->twIntervals->clear();
    QList<QTreeWidgetItem *> items = ui->twArchived->selectedItems();
    foreach(QTreeWidgetItem *it, items)
        m_thdb->getAmtData(it->text(0));
}

void HdbTest::populateSourcesList(const QStringList &list)
{
    qDebug() << QThread::currentThread() << __FUNCTION__ << "entering main loop";

    ui->twArchived->clear();
    foreach(QString s, list)
        new QTreeWidgetItem(ui->twArchived, QStringList() << s);
}

void HdbTest::updateAmtData(const QString& source, const AMTData& data)
{
     QList<QTreeWidgetItem *> items = ui->twIntervals->findItems(source, Qt::MatchExactly);
     QTreeWidgetItem *parentIt = NULL;
     if(items.size() == 1)
         parentIt = items.first();
     else if(!items.size())
         parentIt  = new QTreeWidgetItem(ui->twIntervals, QStringList() << source);

     if(parentIt)
        new QTreeWidgetItem(parentIt, QStringList() << data.startDate.toString("yyyy-MM-dd hh:mm:ss")
                           << data.stopDate.toString("yyyy-MM-dd hh:mm:ss") <<
                          data.archiver<< QString::number(
                          data.perMod)<< QString::number(data.perPerMod));

     if(parentIt)
         parentIt->setExpanded(true);

}

void HdbTest::slotItemClicked(QTreeWidgetItem *, int)
{


}
