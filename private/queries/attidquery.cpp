#include "attidquery.h"
#include <macros.h>
#include <QSqlQuery>
#include <QVariant>
#include <QtDebug>
#include <QThread>

AttIdQuery::AttIdQuery(QObject *parent) : DbQueryInterface(parent)
{
    m_queryId = 0;
}

void AttIdQuery::notify(const QSqlQuery& constQuery, const int queryId)
{
    QDateTime startDt, stopDt;
    QString source;
    IdInfo::Reason reason = IdInfo::None;
    /* assume it is not present into the db (the query is empty set) */
    bool archived = false;

    QSqlQuery q(constQuery);
    QueryDataInfo *qdi = m_idInfoList.getQueryDataInfo(queryId);
    IdInfo *ii = static_cast<IdInfo *>(qdi);
    if(ii)
    {
        startDt = ii->startDateTime;
        stopDt  = ii->stopDateTime;
        source  = ii->source;
        reason  = ii->reason();
    }
    else
        qDebug() << "IdInfo is null " << queryId;

    while(q.next())
    {
        archived = true;
        qDebug() << this << QThread::currentThread() << __FUNCTION__ <<
                    "emit " << q.value(0).toString() <<
                    q.value(1).toInt() << "writable" << q.value(2).toBool();
        if(ii)
        {
            /* according to the reason the id was requested, emit adequate signals */
            if(startDt < stopDt && reason == IdInfo::Data)
                emit idReadyForData(q.value(0).toString(), q.value(1).toInt(), q.value(2).toBool(), startDt, stopDt);
            if(reason == IdInfo::Amt)
                emit idReadyForAmt(q.value(0).toString(), q.value(1).toInt());
            if(reason == IdInfo::Apt)
                emit idReadyForApt(q.value(0).toString(), q.value(1).toInt());
            if(reason == IdInfo::All)
                emit idReady(q.value(0).toString(), q.value(1).toInt());

            m_idInfoList.deleteQueryDataInfo(queryId);
        }
    }
    if(reason == IdInfo::isArchived)
    {
        emit isArchived(source, archived);
    }

}

QString AttIdQuery::query(int id) const
{
    QString q;
    QueryDataInfo *ii = m_idInfoList.getQueryDataInfo(id);
    if(ii)
        q = ii->query();
    qDebug() << this << QThread::currentThread() << __FUNCTION__ << q;
    return q;
}

unsigned AttIdQuery::getId(const QString& source, const QDateTime& startDt, const QDateTime& stopDt,
                           IdInfo::Reason reason)
{
    m_queryId++;
    IdInfo *ii = new IdInfo(m_queryId, source, reason);
    ii->startDateTime = startDt;
    ii->stopDateTime = stopDt;
    m_idInfoList.addQueryDataInfo(ii);
    return m_queryId;
}

unsigned AttIdQuery::getId(const QString& source, IdInfo::Reason reason)
{
    m_queryId++;
    IdInfo *ii = new IdInfo(m_queryId, source, reason);
    m_idInfoList.addQueryDataInfo(ii);
    return m_queryId;
}

