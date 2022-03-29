#include "aptquery.h"
#include "aptdata.h"
#include "aptinfo.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>

#include <QThread>
#include <QtDebug>

APTQuery::APTQuery(QObject *parent) : DbQueryInterface(parent)
{
    m_queryId = 0;
}


void APTQuery::notify(const QSqlQuery& constQuery, const int queryId)
{
    /* Query was (aptinfo.cpp):
     * "SELECT ID,time,description,label,unit,standard_unit,display_unit,"
     * "format,min_value,max_value,min_alarm,max_alarm FROM apt WHERE ID=%1"
     */
    APTData aptData;
    QSqlQuery q(constQuery);
    if(q.next() && q.record().count() == 12)
    {
        QueryDataInfo *qdi = m_idInfoList.getQueryDataInfo(queryId);
        APTInfo *ii = static_cast<APTInfo *>(qdi);

        aptData.attId = q.value(0).toInt();
        aptData.time = q.value(1).toString();
        aptData.description = q.value(2).toString();
        aptData.label = q.value(3).toString();
        aptData.unit = q.value(4).toString();
        aptData.standardUnit = q.value(5).toString();
        aptData.displayUnit = q.value(6).toString();
        aptData.format = q.value(7).toString();
        aptData.minValue = q.value(8).toDouble();
        aptData.maxValue = q.value(9).toDouble();
        aptData.minAlarm = q.value(10).toDouble();
        aptData.maxAlarm = q.value(11).toDouble();
        emit aptReady(ii->source, aptData);
        m_idInfoList.deleteQueryDataInfo(queryId);
    }
}

QString APTQuery::query(int queryId) const
{
    QString q;
    QueryDataInfo *ii = m_idInfoList.getQueryDataInfo(queryId);
    if(ii)
        q = ii->query();
    qDebug() << this << QThread::currentThread() << __FUNCTION__ << q;
    return q;
}

unsigned APTQuery::getAPTInfo(unsigned attributeID, const QString& src)
{
    m_queryId++;
    APTInfo *aptInfo = new APTInfo(m_queryId, attributeID, src);
    m_idInfoList.addQueryDataInfo(aptInfo);
    return m_queryId;
}
