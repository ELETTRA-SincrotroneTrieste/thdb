#ifndef DATAINFO_H
#define DATAINFO_H

#include <QString>
#include <QList>
#include <QDateTime>
#include "querydatainfo.h"

class DataInfo : public QueryDataInfo
{
public:
    DataInfo();

    DataInfo(int query_id, unsigned int att_id, bool writable, const QString& source);

    QString source;

    unsigned attId;

    QString query() const;

    /* start and stop date time are passed through signals/slots */
    QDateTime startDateTime, stopDateTime;

    bool writable;

};

#endif // DATAINFO_H
