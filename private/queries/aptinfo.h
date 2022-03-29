#ifndef APTINFO_H
#define APTINFO_H

#include <QString>
#include "querydatainfo.h"

class APTInfo : public QueryDataInfo
{
public:
    APTInfo(int queryId, unsigned att_id, const QString& source);
    unsigned attId;
    QString source;
    QString query() const;

};


#endif // APTINFO_H
