#ifndef ATTIDQUERY_H
#define ATTIDQUERY_H

#include "dbqueryinterface.h"
#include "idinfo.h"
#include <QStringList>
#include <QDateTime>

class AttIdQuery : public DbQueryInterface
{
    Q_OBJECT
public:

    AttIdQuery(QObject *parent = NULL);

    virtual void notify(const QSqlQuery&, const int eventId);

    virtual QString query(int id) const;

signals:
    void idReadyForData(const QString& source, int id, int writable, const QDateTime& startDt, const QDateTime& stopDt);

    void idReady(const QString& source, int id);
    void idReadyForAmt(const QString& source, int id);
    void idReadyForApt(const QString& source, int id);
    void idReadyForUser(const QString& source, int it, int userReason);

    void isArchived(const QString& source, bool);

public slots:

    unsigned getId(const QString& source, const QDateTime& startDt, const QDateTime& stopDt,
                   IdInfo::Reason = IdInfo::Data);

    unsigned getId(const QString& source, IdInfo::Reason reason);

private:

    QueryDataInfoList m_idInfoList;

    QStringList m_sources;

    QDateTime m_SavedStartDateTime, m_SavedStopDateTime;

    unsigned m_queryId;

};

#endif // ATTIDQUERY_H
