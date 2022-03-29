#include "amtquery.h"
#include "amtdata.h"
#include "amtinfo.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>

#include <QThread>
#include <QtDebug>

AMTQuery::AMTQuery(QObject *parent) : DbQueryInterface(parent)
{
    m_queryId = 0;
}


void AMTQuery::notify(const QSqlQuery& constQuery, const int queryId)
{
    /* Query was (amtinfo.cpp):
     * "SELECT ID,archiver,start_date,stop_date,per_mod,per_per_mod FROM amt WHERE ID=%1"
     */

    QSqlQuery q(constQuery);
    while(q.next() && q.record().count() == 6)
    {
        QueryDataInfo *qdi = m_idInfoList.getQueryDataInfo(queryId);
        AMTInfo *ii = static_cast<AMTInfo *>(qdi);
        AMTData amtData;
        amtData.attId = q.value(0).toInt();
        amtData.archiver = q.value(1).toString();
        amtData.startDate = QDateTime::fromString(q.value(2).toString(), "yyyy-MM-ddThh:mm:ss");
        amtData.stopDate = QDateTime::fromString(q.value(3).toString(), "yyyy-MM-ddThh:mm:ss");
        amtData.perMod = q.value(4).toInt();
        amtData.perPerMod = q.value(5).toInt();
        qDebug() << "amtReady " << amtData.startDate << amtData.stopDate;
        emit amtReady(ii->source, amtData);
    }
    m_idInfoList.deleteQueryDataInfo(queryId);
}

QString AMTQuery::query(int queryId) const
{
    QString q;
    QueryDataInfo *ii = m_idInfoList.getQueryDataInfo(queryId);
    if(ii)
        q = ii->query();
    qDebug() << this << QThread::currentThread() << __FUNCTION__ << q;
    return q;
}

unsigned AMTQuery::getAMTInfo(unsigned attributeID, const QString& src)
{
    m_queryId++;
    AMTInfo *amtInfo = new AMTInfo(m_queryId, attributeID, src);
    m_idInfoList.addQueryDataInfo(amtInfo);
    return m_queryId;
}
