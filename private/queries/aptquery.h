#ifndef ADTQUERY_H
#define ADTQUERY_H

#include <QString>
#include "querydatainfo.h"
#include "dbqueryinterface.h"

class APTData;

class APTQuery : public DbQueryInterface
{
    Q_OBJECT

public:
    APTQuery(QObject *parent = NULL);

    virtual void notify(const QSqlQuery&, const int eventId);

    virtual QString query(int id) const;

signals:
    void aptReady(const QString& source, const APTData&);

public slots:

    unsigned getAPTInfo(unsigned attributeID, const QString& source);

private:

    QueryDataInfoList m_idInfoList;

    unsigned m_queryId;
};

#endif // ADTQUERY_H
