#include "scalarnumbersquery.h"
#include "datainfo.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <macros.h>

#include <QtDebug>
#include <QThread>

ScalarNumbersQuery::ScalarNumbersQuery(QObject *parent) : DbQueryInterface(parent)
{
     m_currentQueryId = 0;
     m_writable = true;
}

void ScalarNumbersQuery::notify(const QSqlQuery&constQuery, const int queryId)
{
    QSqlQuery q(constQuery);
    qDebug() << this << QThread::currentThread() << __FUNCTION__;

    QVector<double> xdata, ydata, yWData;

    DataInfo *di = static_cast<DataInfo *>(m_dataInfoList.getQueryDataInfo(queryId));
    if(di)
    {
        if(q.next() && q.isValid())
        {
            QSqlRecord record = q.record();
            if(record.count() == 2 && record.field(0).name() == "time"
                    && record.field(1).name() == "value")
            {
                do{
                    xdata << QDateTime::fromString(q.value(0).toString(), "yyyy-MM-dd hh:mm:ss").toTime_t();
                    ydata << q.value(1).toDouble();
                }while(q.next());

                emit dataAvailable(di->source , xdata, ydata);
            }
            else if(record.count() == 3 && record.field(0).name() == "time"
                    && record.field(1).name() == "read_value"
                    && record.field(2).name() == "write_value")
            {
                qDebug() << "--> recognized time/read_value/write_value name in query " << queryId;
                do{
                    xdata << QDateTime::fromString(q.value(0).toString(), "yyyy-MM-dd hh:mm:ss").toTime_t();
                    ydata << q.value(1).toDouble();
                    yWData << q.value(2).toDouble();
                }while(q.next());

                emit dataAvailable(di->source, xdata, ydata);
                emit dataSetPointAvailable(di->source, xdata, yWData);
            }
            else
                perr("ScalarNumbersQuery::notify(): error: query \"%s\" has unsupported fields",
                     qstoc(q.executedQuery()));
        }
        else
            qDebug() << "query not valid";

        m_dataInfoList.deleteQueryDataInfo(queryId);
    }
}

QString ScalarNumbersQuery::query(int queryId) const
{
    QString query;
    QueryDataInfo *di = m_dataInfoList.getQueryDataInfo(queryId);
    if(di)
        query = di->query();
    qDebug() << this << QThread::currentThread() << __FUNCTION__ << query;
    return query;
}

unsigned int  ScalarNumbersQuery::getData(int attributeId, const QString& source,
      const QDateTime& start, const QDateTime &stop)
{
    m_currentQueryId++;
    DataInfo *di = new DataInfo(m_currentQueryId, attributeId, m_writable, source);
    di->startDateTime = start;
    di->stopDateTime = stop;
    m_dataInfoList.addQueryDataInfo(di);
    return m_currentQueryId;
}


