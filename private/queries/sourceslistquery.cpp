#include "sourceslistquery.h"
#include <QStringList>
#include <QSqlQuery>
#include <QVariant>

/* for debug */
#include <QtDebug>
#include <QThread>

SourcesListQuery::SourcesListQuery(QObject *parent) : DbQueryInterface(parent)
{

}

void SourcesListQuery::notify(const QSqlQuery& constQuery, const int eventId)
{
    Q_UNUSED(eventId);
    qDebug() << this <<  QThread::currentThread() << __FUNCTION__ << "query sze" << constQuery.size();
    QStringList result;
    QSqlQuery q(constQuery);
    while(q.next())
        result << q.value(0).toString();
    emit listReady(result);
}

QString SourcesListQuery::query(int) const
{
    QString query;
    qDebug() << this <<  QThread::currentThread() << __FUNCTION__;
    query = "select adt.full_name from adt,apt where apt.ID=adt.ID ORDER BY adt.full_name";
    return query;
}

