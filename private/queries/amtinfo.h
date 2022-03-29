#ifndef APTINFO_H
#define APTINFO_H

#include <QString>
#include <QDateTime>
#include "querydatainfo.h"

class AMTInfo : public QueryDataInfo
{
public:
    AMTInfo(int queryId, unsigned att_id, const QString& source);
    unsigned attId;
    QString source;

    /** \brief get amt info between a start and stop date.
      *
      * If not specified, the query will ask for all available data.
      * If only startDateTime is specified, the query will ask for data whose start_date
      * is greater than startDateTime.
      * If only stopDateTime is specified, then the query will ask for data whose
      * stop_date is before the stopDateTime.
      * If both start and stop date time are specified, then data will be fetch inside
      * the interval provided.
      */
    QDateTime startDateTime, stopDateTime;
    QString query() const;

};


#endif // APTINFO_H
