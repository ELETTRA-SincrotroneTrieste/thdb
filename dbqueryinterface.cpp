#include "dbqueryinterface.h"
#include "dbqueryevent.h"
#include "queryresultevent.h"
#include "thdbbase.h"
#include <QObject>
#include <QSqlQuery>

/* for debug */
#include <QtDebug>
#include <QThread>

DbQueryInterface::DbQueryInterface(QObject *parent) :
    QObject(parent)
{

}

void DbQueryInterface::execute(int id)
{
    emit executeSignal(m_interfaceName, id);
}

bool DbQueryInterface::event(QEvent *e)
{
    if(e->type() == QResultEvent) /* #defined in queryresultevent.h */
    {
        qDebug() << this <<  QThread::currentThread() << __FUNCTION__ << "QResultEvent: calling notify()";
        QueryResultEvent *qre = static_cast<QueryResultEvent *>(e);
        notify(qre->result(), qre->eventId());
        return true;
    }
    return QObject::event(e);
}
