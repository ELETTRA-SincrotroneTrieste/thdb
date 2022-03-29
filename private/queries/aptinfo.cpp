#include "aptinfo.h"

APTInfo::APTInfo(int queryId, unsigned att_id, const QString& src) :
    QueryDataInfo(queryId)
{
    attId = att_id;
    source = src;
}

QString APTInfo::query() const
{
    QString  query;
    query = QString("SELECT ID,time,description,label,unit,standard_unit,display_unit,"
                    "format,min_value,max_value,min_alarm,max_alarm FROM apt WHERE ID=%1")
                .arg(attId);
    return query;
}
