#include "querydatainfo.h"
#include <QtDebug>

QueryDataInfo::QueryDataInfo(int query_id)
{
    queryId = query_id;
}

QueryDataInfo::~QueryDataInfo()
{
    qDebug() << __FUNCTION__ << "queryId " << queryId << "destructor";
}

bool QueryDataInfo::operator ==(const QueryDataInfo &other) const
{
    return queryId == other.queryId;
}

QueryDataInfo* QueryDataInfoList::getQueryDataInfo(int queryId) const
{
   foreach(QueryDataInfo *ii, m_queryDataInfoList)
   {
       if(ii->queryId == queryId)
           return ii;
   }
   return NULL;
}

void QueryDataInfoList::removeQueryDataInfo(int queryId)
{
    QueryDataInfo* foundDataInfo;
    foreach(QueryDataInfo *ii, m_queryDataInfoList)
        if(ii->queryId == queryId)
            foundDataInfo = ii;
    if(!foundDataInfo->isNull())
    {
        m_queryDataInfoList.removeAll(foundDataInfo);
        qDebug() << "-- removed info " << foundDataInfo->queryId
                 << "size of list" << m_queryDataInfoList.size();
    }
    else
        qDebug() << "error: DataInfo with query id " << queryId << "not found!";
}

void QueryDataInfoList::deleteQueryDataInfo(int queryId)
{
    QueryDataInfo* foundDataInfo = NULL;
    foreach(QueryDataInfo *ii, m_queryDataInfoList)
        if(ii->queryId == queryId)
            foundDataInfo = ii;
    if(foundDataInfo)
    {
        m_queryDataInfoList.removeAll(foundDataInfo);
        qDebug() << "-- delete info " << foundDataInfo->queryId
                 << "size of list" << m_queryDataInfoList.size();
        delete foundDataInfo;
    }
    else
        qDebug() << "error: DataInfo with query id " << queryId << "not found!";
}
