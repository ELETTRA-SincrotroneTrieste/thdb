#include "hdbproxy.h"
#include <macros.h>
#include <QStringList>
#include <QtDebug>
#include <QCoreApplication>

HDbProxy *HDbProxy::_instance = NULL;

HDbProxy* HDbProxy::instance()
{
    if(!_instance)
        _instance = new HDbProxy(0);
    return _instance;
}

HDbProxy::HDbProxy(QObject *parent) : DbProxy(parent)
{
    m_dbError = false;
    m_user = "hdbbrowser";
    m_pass = "hdbbrowser";
    m_schema = "hdb";
    char *hdb_server = getenv("HDB_SERVER");
    if(!hdb_server)
    {
        m_dbError = true;
    }
    else
    {
        QString hdbServer(hdb_server);
        QStringList hdbServerPort = hdbServer.split(":");
        (hdbServerPort.size() == 2) ? (m_dbError = false) : (m_dbError = true);
        if(!m_dbError)
        {
            bool ok;
            m_host = hdbServerPort.first();
            m_port = hdbServerPort.last().toInt(&ok);
            m_dbError = !ok;
        }
    }
    if(m_dbError)
    {

        m_lastErrorMessage = "HDbProxy: error in HDB_SERVER environment syntax";
        perr("%s", qstoc(m_lastErrorMessage));
        emit error(m_lastErrorMessage);
    }
    else
        start();
}

void HDbProxy::run()
{
    m_sqlDb = QSqlDatabase::addDatabase("QMYSQL");
    m_sqlDb.setHostName(m_host);
    m_sqlDb.setPassword(m_pass);
    m_sqlDb.setUserName(m_user);
    m_sqlDb.setDatabaseName(m_schema);
    m_sqlDb.setPort(m_port);

    m_dbError = !m_sqlDb.open();
    if(m_dbError)
    {
        m_lastErrorMessage = QString("HDbProxy: failed to connect to database with host \"%1\", port %2, schema \"%3\""
                      "user \"%4\" password \"%5\"").arg(m_host).arg(m_port).arg(m_schema).arg(m_user).arg(m_pass);
        emit error(m_lastErrorMessage);
        perr("%s", qstoc(m_lastErrorMessage));
    }
    else
    {
        pok("Connected to database host \"%s\", port %d, schema \"%s\" user \"%s\" password \"%s\"",
            qstoc(m_host), m_port, qstoc(m_schema), qstoc(m_user), qstoc(m_pass));

        DbProxy::run();
    }
}
