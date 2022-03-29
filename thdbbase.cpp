#include "thdbbase.h"
#include "private/thdbbaseprivate.h"

#include <dbproxy.h>
#include <macros.h>
#include "dbqueryinterface.h"
#include <QHash>
#include <dbconnectionparams.h>

QString THdbBase::version()
{
    return QString(THDB_VERSION);
}

THdbBase::THdbBase(QObject *parent) :
        QObject(parent)
{
    d_ptr = new  THdbBasePrivate(this);
}

THdbBase::~THdbBase()
{
    delete d_ptr;
}

THdbBase::Type THdbBase::type() const
{
    return d_ptr->type();
}

void THdbBase::setConnectionParameters(const DbConnectionParams &dbCp)
{
    d_ptr->setType(d_ptr->type(), dbCp);
}

void THdbBase::setType(Type t, const DbConnectionParams &dbCp)
{
    d_ptr->setType(t, dbCp);
    emit databaseTypeChanged(t);
}

void THdbBase::registerDbQuery(const QString& name, DbQueryInterface *dbQuery)
{
    connect(dbQuery, SIGNAL(executeSignal(const QString&, int )),
            this, SLOT(executeQuery(const QString&, int )));
    dbQuery->setInterfaceName(name);
    d_ptr->registerDbQuery(name, dbQuery);
}

void THdbBase::unregisterDbQuery(const QString& name)
{
    d_ptr->unregisterDbQuery(name);
}

DbQueryInterface *THdbBase::dbQueryInterface(const QString& name)
{
    QHash <QString, DbQueryInterface *> hash = d_ptr->queryInterfaceHash();
    if(hash.contains(name))
        return hash.value(name);
    return NULL;
}

QStringList THdbBase::registeredQueryInterfaces() const
{
    return d_ptr->queryInterfaceHash().keys();
}

void THdbBase::executeQuery(const QString& interfaceName, int id)
{
    qDebug() << this << QThread::currentThread() << __FUNCTION__;
    printf("\e[1;36mTHdbBase::executeQuery ifname %s type %d id %d\e[0m\n", qstoc(interfaceName),
           type(), id);
    d_ptr->executeQuery(interfaceName, id);
}


