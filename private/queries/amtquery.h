#ifndef AMTQUERY_H
#define AMTQUERY_H

#include <QString>
#include "querydatainfo.h"
#include "dbqueryinterface.h"

class AMTData;

class AMTQuery : public DbQueryInterface
{
    Q_OBJECT

public:
    AMTQuery(QObject *parent = NULL);

    virtual void notify(const QSqlQuery&, const int eventId);

    virtual QString query(int id) const;

signals:
    void amtReady(const QString& source, const AMTData&);

public slots:

    unsigned getAMTInfo(unsigned attributeID, const QString& source);

private:

    QueryDataInfoList m_idInfoList;

    unsigned m_queryId;
};

#endif // ADTQUERY_H
