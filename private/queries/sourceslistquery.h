#ifndef SOURCESLISTQUERY_H
#define SOURCESLISTQUERY_H

#include "dbqueryinterface.h"

class QStringList;

class SourcesListQuery : public DbQueryInterface
{
    Q_OBJECT
public:
    SourcesListQuery(QObject *parent = NULL);

    virtual void notify(const QSqlQuery&, const int eventId);

    virtual QString query(int id = -1) const;

public slots:

signals:
    void listReady(const QStringList &);


};

#endif // SOURCESLISTQUERY_H
