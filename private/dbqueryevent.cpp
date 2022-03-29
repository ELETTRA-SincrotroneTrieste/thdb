#include "dbqueryevent.h"
#include "queryresultevent.h"
#include <QtDebug>
#include <QThread>

DbQueryEvent::DbQueryEvent(DbQueryInterface *queryIf, int id)
{
    m_queryIf = queryIf;
    m_id = id;
    m_type = DatabaseQuery;
}

DbQueryEvent::~DbQueryEvent()
{
    qDebug() << this <<  QThread::currentThread() << __FUNCTION__ << "destructor";
}
