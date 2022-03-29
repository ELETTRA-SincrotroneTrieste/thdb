#ifndef IDINFO_H
#define IDINFO_H

#include <QString>
#include <QList>
#include <QDateTime>
#include "querydatainfo.h"

class IdInfo : public QueryDataInfo
{
public:
    enum Reason { None = 0x0000, Adt = 0x0001, Amt = 0x0002, Apt = 0x0004,
                  Data =0x0008, isArchived = 0x0010, All = 0xFF };

    IdInfo(unsigned query_id = -1, const QString& source = QString(), Reason = All);

    QString source;

    QString query() const;

    Reason reason() const { return m_reason; }

    /* start and stop date time are passed through signals/slots */
    QDateTime startDateTime, stopDateTime;

private:
    Reason m_reason;
};



#endif // IDINFO_H
