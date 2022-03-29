#include "datainfo.h"
#include <QtDebug>
#include <QThread>
#include <macros.h>

DataInfo::DataInfo()
{
    queryId = 0;
}

DataInfo::DataInfo(int query_id, unsigned att_id, bool writab, const QString& src) : QueryDataInfo(query_id)
{
    attId = att_id;
    source = src;
    writable = writab;
}

QString DataInfo::query() const
{
    QString start, stop, query;
    start = startDateTime.toString("yyyy-MM-dd hh:mm:ss");
    stop  =  stopDateTime.toString("yyyy-MM-dd hh:mm:ss");
    if(writable)
        query = QString("SELECT time,read_value,write_value FROM att_%1 WHERE time >= '%2' AND time <= '%3' "
                    " UNION (SELECT '%2' AS time,read_value,write_value FROM att_%1 WHERE time < '%2' ORDER BY att_%1.time DESC LIMIT 1) ORDER BY time").
            arg(attId, 5, 10, QChar('0')).arg(start).arg(stop);
    else
        query = QString("SELECT time,value FROM att_%1 WHERE time >= '%2' AND time <= '%3' "
                    " UNION (SELECT '%2' AS time,value FROM att_%1 WHERE time < '%2' ORDER BY att_%1.time DESC LIMIT 1) ORDER BY time").
            arg(attId, 5, 10, QChar('0')).arg(start).arg(stop);

    return query;
}
