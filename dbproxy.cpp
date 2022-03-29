#include "dbproxy.h"
#include <QMetaType>
#include <QtDebug>
#include <QTimer>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <macros.h>
#include <QCoreApplication>
#include "aptinfo.h"
#include "dbqueryevent.h"
#include "dbqueryinterface.h"
#include "queryresultevent.h"
#include "switchdatabaseevent.h"

DbProxy* DbProxy::_instance = NULL;

DbProxy* DbProxy::instance(QObject *parent)
{
    if(!_instance)
        _instance = new DbProxy(parent);
    return _instance;
}

DbProxy::DbProxy(QObject *parent) :
    QThread(parent)
{
    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(appAboutToQuit()),
          Qt::DirectConnection);

    m_queryCnt = 0;

    /* start the thread */
    start();
}

DbProxy::~DbProxy()
{
    qDebug() << __FUNCTION__ << "waiting...";
    wait();
    qDebug() << __FUNCTION__ << "waited.";
}

void DbProxy::run()
{
    QTimer *processTimer = new QTimer(0);
    QTimer *closeTimer = new QTimer(0);

    m_sqlDb = QSqlDatabase::addDatabase("QMYSQL");

    connect(this, SIGNAL(exitLoop()), closeTimer, SIGNAL(timeout()), Qt::QueuedConnection);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(close()), Qt::DirectConnection);

    connect(this, SIGNAL(processRequest()), processTimer, SIGNAL(timeout()), Qt::QueuedConnection);
    connect(processTimer, SIGNAL(timeout()), this, SLOT(process()), Qt::DirectConnection);
    /* event loop */
    QThread::currentThread()->setObjectName("DbProxy Thread");
    qDebug() << this <<  QThread::currentThread() << __FUNCTION__ << "entering main loop";
    /* some events might have been accumulated in the queue, let's check as soon as we start */
    processTimer->setSingleShot(true);
    processTimer->setInterval(300);
    processTimer->start();
    exec();
    qDebug() << this <<  QThread::currentThread() << __FUNCTION__ << "leaving main loop";
}

void DbProxy::addQueryEvent(DbQueryEvent *dbqe)
{
    qDebug() << this << objectName() << __FUNCTION__;
    QWriteLocker wLock(&m_rwLock);
    m_eventQueue << dbqe;
    emit processRequest();
}

void DbProxy::m_switchDatabase()
{
    /* called by process() in DbThread. Lock is held by process()
     * before calling this private method. No need to lock here.
     */
    if(m_sqlDb.isOpen())
        m_sqlDb.close();

    pinfo("switching database to host \"%s\"  name \"%s\"\e[0m\n",
           qstoc(m_dbConnectionParams.dbHost), qstoc(m_dbConnectionParams.dbName));
    m_sqlDb.setHostName(m_dbConnectionParams.dbHost);
    m_sqlDb.setPassword(m_dbConnectionParams.dbPassword);
    m_sqlDb.setUserName(m_dbConnectionParams.dbUser);
    m_sqlDb.setDatabaseName(m_dbConnectionParams.dbName);
    m_sqlDb.setPort(m_dbConnectionParams.dbPort);

    /* auto reconnect */
    m_sqlDb.setConnectOptions("MYSQL_OPT_RECONNECT=1");

    m_dbError = !m_sqlDb.open();
    if(m_dbError)
    {
        m_lastErrorMessage = QString("HDbProxy: failed to connect to database with host \"%1\", port %2, schema \"%3\""
                                     "user \"%4\" password \"%5\"").
                arg(m_sqlDb.hostName()).arg(m_sqlDb.port()).
                arg(m_sqlDb.databaseName()).arg(m_sqlDb.userName()).
                arg(m_sqlDb.password());
        emit databaseError("DbProxy: db open failed", m_lastErrorMessage);
        emit connectionInfo(m_lastErrorMessage);
        perr("m_switchDatabase: database open error: %s", qstoc(m_lastErrorMessage));
    }
    else
    {
        emit databaseOpen();
        QString connInfo = QString("Connected to host \"%1\" user \"%2\" db \"%3\"").arg(
                    m_sqlDb.hostName(), m_sqlDb.userName(), m_sqlDb.databaseName());
        emit connectionInfo(connInfo);
        pok("%s", qstoc(connInfo));
    }

}

void DbProxy::setDatabase(const DbConnectionParams& dbconPar)
{
    /* using DbQueryEvent / SwitchDatabaseInterface ensures that all operations
     * invoked on the thread are requested in the desired order.
     */
    addQueryEvent(new SwitchDatabaseEvent(dbconPar));
}

void DbProxy::process()
{
    QWriteLocker wLocker(&m_rwLock);
    qDebug() << this << objectName() << __FUNCTION__ << "eventQueueSize" << m_eventQueue.size();
    while(!m_eventQueue.isEmpty())
    {
        /* dequeue the event */
        DbQueryEvent *e = m_eventQueue.dequeue();
        if(e->type() == DbQueryEvent::SwitchDatabase)
        {
            m_dbConnectionParams = static_cast<SwitchDatabaseEvent *>(e)->dbConnectionParams;
            m_switchDatabase();
        }
        else if(!m_sqlDb.isOpen())
        {
            perr("DbProxy::process(): database is not open.\n  Have you called setDatabase?");
            emit databaseError("DbProxy: process", "database is not open");
            emit connectionInfo("DbProxy: process: database is not open");
        }
        else
        {
            bool result;
            QSqlQuery q;
            q.setNumericalPrecisionPolicy(QSql::HighPrecision);
            QString query;
            qDebug() << __FUNCTION__ << "query" << e->queryInterface()->query(e->id())
                     << "db" << m_sqlDb.databaseName() << "host" << m_sqlDb.hostName() <<
                        "query id" << e->id() << "user" << m_sqlDb.userName();
            emit querySent();
            query = e->queryInterface()->query(e->id());
            if(!query.isEmpty())
                result = q.exec(query);
            emit queryExecuted(result);
            if(!result)
            {
                emit databaseError("DbProxy: process: exec()", q.lastError().text());
                emit connectionInfo(q.lastError().text());
            }
            m_queryCnt++;
            QCoreApplication::postEvent(e->queryInterface(), new QueryResultEvent(q, e->id()));
        }
        /* delete the event */
        delete e;
    }
}

void DbProxy::appAboutToQuit()
{
    qDebug() << __FUNCTION__ << QThread::currentThread() << this;
    emit exitLoop();
}

void DbProxy::close()
{
    qDebug() << this <<  QThread::currentThread() << __FUNCTION__;
    if(m_sqlDb.isOpen())
        m_sqlDb.close();
    QSqlDatabase::removeDatabase(m_sqlDb.connectionName());
    quit();
}
