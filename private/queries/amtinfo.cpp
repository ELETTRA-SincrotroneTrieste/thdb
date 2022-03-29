#include "amtinfo.h"

AMTInfo::AMTInfo(int queryId, unsigned att_id, const QString& src) :
    QueryDataInfo(queryId)
{
    attId = att_id;
    source = src;
}

QString AMTInfo::query() const
{
    QString  query;
    if(startDateTime.isValid() && stopDateTime.isValid())
    {
        query = QString("SELECT ID,archiver,start_date,stop_date,per_mod,per_per_mod FROM amt"
                    " WHERE ID=%1 AND start_date >='%2' AND stop_date <= '%3'")
                .arg(attId).arg(startDateTime.toString("yyyy-MM-ddThh:mm:ss")
                .arg(stopDateTime.toString("yyyy-MM-ddThh:mm:ss")));
    }
    else if(startDateTime.isValid() && stopDateTime.isNull())
    {
        query = QString("SELECT ID,archiver,start_date,stop_date,per_mod,per_per_mod FROM amt"
                    " WHERE ID=%1 AND start_date >='%2'")
                .arg(attId).arg(startDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    }
    else if(startDateTime.isNull() && stopDateTime.isValid())
    {
        query = QString("SELECT ID,archiver,start_date,stop_date,per_mod,per_per_mod FROM amt"
                    " WHERE ID=%1 AND stop_date <= '%2'")
                .arg(attId).arg(stopDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    }
    else
    {
        query = QString("SELECT ID,archiver,start_date,stop_date,per_mod,per_per_mod FROM amt"
                    " WHERE ID=%1").arg(attId);
    }
    return query;
}
