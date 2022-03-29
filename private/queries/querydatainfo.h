#ifndef QUERYDATAINFO_H
#define QUERYDATAINFO_H

#include <QList>
#include <QString>

class QueryDataInfo
{
public:
    QueryDataInfo(int query_id = -1);

    virtual ~QueryDataInfo();

    virtual QString query() const = 0;

    virtual bool operator ==(const QueryDataInfo &other) const;

    virtual bool isNull() const { return queryId == -1; }

    int queryId;
};

class QueryDataInfoList
{
public:
    QueryDataInfoList() {};

    void addQueryDataInfo(QueryDataInfo* ii) { m_queryDataInfoList << ii; }

    void removeQueryDataInfo(int queryId);

    void deleteQueryDataInfo(int queryId);

    QueryDataInfo* getQueryDataInfo(int queryId) const;

private:
    QList<QueryDataInfo *> m_queryDataInfoList;
};

#endif // QUERYDATAINFO_H
