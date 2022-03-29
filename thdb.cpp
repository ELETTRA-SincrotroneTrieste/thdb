#include "thdb.h"
#include "queries/sourceslistquery.h"
#include "queries/scalarnumbersquery.h"
#include "queries/attidquery.h"
#include "queries/aptquery.h"
#include "queries/aptdata.h"
#include "queries/amtquery.h"
#include "queries/amtdata.h"
#include <dbproxy.h>
#include <QHash>
#include <QtDebug>

THdb::THdb(QObject *parent) : THdbBase(parent)
{
    qRegisterMetaType<APTData>("APTData");
    qRegisterMetaType<AMTData>("AMTData");

    registerDbQuery("archivedSources", new SourcesListQuery(this));
    connect(dbQueryInterface("archivedSources"),
            SIGNAL(listReady(const QStringList &)),
            this, SIGNAL(archivedSourcesListReady(const QStringList &)));

    registerDbQuery("attId", new AttIdQuery(this));
    registerDbQuery("scalarNumbers", new ScalarNumbersQuery(this));
    registerDbQuery("apt", new APTQuery(this));
    registerDbQuery("amt", new AMTQuery(this));

    connect(dbQueryInterface("attId"),
            SIGNAL(idReadyForData(const QString&, int, int, const QDateTime&, const QDateTime&)),
                   this, SLOT(d_getScalarData(const QString&, int, int, const QDateTime&, const QDateTime&)));

    connect(dbQueryInterface("attId"),
            SIGNAL(idReadyForApt(const QString&, int)), this, SLOT(d_getAptData(const QString&, int)));

    connect(dbQueryInterface("attId"),
            SIGNAL(idReadyForAmt(const QString&, int)), this, SLOT(d_getAmtData(const QString&, int)));

    connect(dbQueryInterface("attId"),
            SIGNAL(isArchived(const QString&, bool)), this, SIGNAL(isArchived(const QString&, bool)));

    connect(dbQueryInterface("scalarNumbers"),
            SIGNAL(dataAvailable(const QString&, const QVector<double>&, const QVector<double>&)),
            this,
            SIGNAL(dataAvailable(const QString&, const QVector<double >&, const QVector<double> &)));

    connect(dbQueryInterface("apt"), SIGNAL(aptReady(const QString&, const APTData&)),
            this, SIGNAL(aptDataReady(const QString&, const APTData &)));

    connect(dbQueryInterface("amt"), SIGNAL(amtReady(const QString&, const AMTData&)),
            this, SIGNAL(amtDataReady(const QString&, const AMTData &)));

    connect(dbQueryInterface("scalarNumbers"),
            SIGNAL(dataSetPointAvailable(const QString&, const QVector<double>&, const QVector<double>&)),
            this,
            SIGNAL(dataSetPointAvailable(const QString&, const QVector<double >&, const QVector<double> &)));
}

THdb::~THdb()
{

}

void THdb::d_getScalarData(const QString& source, int attrId, int writable,
                const QDateTime& startDate, const QDateTime& stopDate)
{
    Q_UNUSED(attrId);
    unsigned queryId;
    ScalarNumbersQuery *scalarNQ = static_cast<ScalarNumbersQuery *>(dbQueryInterface("scalarNumbers"));
    scalarNQ->setWritable(writable);
    queryId = scalarNQ->getData(attrId, source, startDate, stopDate);
    scalarNQ->execute(queryId);
}

void THdb::d_getAptData(const QString& source, int attId)
{
    unsigned queryId;
    APTQuery *aptQuery = static_cast<APTQuery *>(dbQueryInterface("apt"));
    queryId = aptQuery->getAPTInfo(attId, source);
    aptQuery->execute(queryId);
}


void THdb::getAptData(const QString& source)
{
    unsigned queryId;
    AttIdQuery *attIdQuery = static_cast<AttIdQuery *>(dbQueryInterface("attId"));
    queryId = attIdQuery->getId(source, IdInfo::Apt);
    attIdQuery->execute(queryId);
}

void THdb::getAmtData(const QString& source)
{
    unsigned queryId;
    AttIdQuery *attIdQuery = static_cast<AttIdQuery *>(dbQueryInterface("attId"));
    queryId = attIdQuery->getId(source, IdInfo::Amt);
    attIdQuery->execute(queryId);
}


void THdb::getData(const QString& source, const QDateTime& startDate, const QDateTime& stopDate)
{
    qDebug() << "Thdb" << __PRETTY_FUNCTION__ << "db type" << type();
    unsigned queryId;
    AttIdQuery *attIdQuery = static_cast<AttIdQuery *>(dbQueryInterface("attId"));
    queryId = attIdQuery->getId(source, startDate, stopDate, IdInfo::Data);
    attIdQuery->execute(queryId);
}

void THdb::isArchived(const QString& source)
{
    unsigned queryId;
    AttIdQuery *attIdQuery = static_cast<AttIdQuery *>(dbQueryInterface("attId"));
    queryId = attIdQuery->getId(source, IdInfo::isArchived);
    attIdQuery->execute(queryId);
}

void THdb::d_getAmtData(const QString& source, int attId)
{
    unsigned queryId;
    AMTQuery *amtQuery = static_cast<AMTQuery *>(dbQueryInterface("amt"));
    queryId = amtQuery->getAMTInfo(attId, source);
    amtQuery->execute(queryId);
}

void THdb::getArchivedSources()
{
    qDebug() << __FUNCTION__;
    dbQueryInterface("archivedSources")->execute();
}
