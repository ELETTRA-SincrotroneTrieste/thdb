#include "thdbbaseprivate.h"
#include "thdbbase.h"
#include "dbproxy.h"
#include "dbqueryevent.h"
#include "dbconnectionparams.h"
#include <macros.h>
#include <QtDebug>

THdbBasePrivate::THdbBasePrivate(THdbBase *parent) : q_ptr(parent)
{
    m_type = THdbBase::Unspecified;
    m_dbProxy = DbProxy::instance();

    /* We rely on AutoConnection as far as different thread signal/slots connections are
     * concerned. The following map signals between dbProxy thread and THdbBase object.
     */
    QObject::connect(m_dbProxy, SIGNAL(databaseOpen()), q_ptr, SIGNAL(databaseOpen()));
    QObject::connect(m_dbProxy, SIGNAL(started()), q_ptr, SIGNAL(dbThreadStarted()));
    QObject::connect(m_dbProxy, SIGNAL(finished()), q_ptr, SIGNAL(dbThreadFinished()));
    QObject::connect(m_dbProxy, SIGNAL(started()), q_ptr, SLOT(onThreadStarted()));
    QObject::connect(m_dbProxy, SIGNAL(queryExecuted(bool)), q_ptr, SIGNAL(queryExecuted(bool)));
    QObject::connect(m_dbProxy, SIGNAL(querySent()), q_ptr, SIGNAL(querySent()));
    QObject::connect(m_dbProxy, SIGNAL(databaseError(const QString&, const QString&)),
                     q_ptr, SIGNAL(databaseError(const QString&, const QString&)));
    QObject::connect(m_dbProxy, SIGNAL(connectionInfo(const QString&)), q_ptr,
                     SIGNAL(dbConnectionInfo(const QString&)));
}

THdbBasePrivate::~THdbBasePrivate()
{
    pinfo("waiting for database thread to exit...");
    m_dbProxy->wait();
    pinfo("database thread exited.");
}

void THdbBasePrivate::setType(THdbBase::Type t, const DbConnectionParams &dbCp)
{
    m_dbProxy->setObjectName(dbCp.dbName + "DatabaseProxy");
    m_dbProxy->setDatabase(dbCp);
    m_type = t; /* user type */
}

void THdbBasePrivate::unregisterDbQuery(const QString& name)
{
    if(m_queryInterfaceHash.contains(name))
        m_queryInterfaceHash.remove(name);
}

void THdbBasePrivate::registerDbQuery(const QString& name, DbQueryInterface *dbQuery)
{
    m_queryInterfaceHash.insert(name, dbQuery);
}

void THdbBasePrivate::executeQuery(const QString& interfaceName, int id)
{
    if(m_queryInterfaceHash.contains(interfaceName))
    {
        DbQueryInterface *queryIf = m_queryInterfaceHash.value(interfaceName);
        DbQueryEvent *queryEvent = new DbQueryEvent(queryIf, id);
        qDebug() << "THdbBasePrivate" << __PRETTY_FUNCTION__
                 << "m_dbProxy: " << m_dbProxy << m_dbProxy->objectName();
        m_dbProxy->addQueryEvent(queryEvent);
    }
}
