#include "dbconnectionparams.h"
#include <stdlib.h>
#include <QStringList>

DbConnectionParams::DbConnectionParams(const QString& dbNam,
                                       const QString& dbU,
                                       const QString& dbPassw,
                                       const QString& dbH,
                                       unsigned short dbPt)
{
    dbName = dbNam;
    dbUser = dbU;
    dbPassword = dbPassw;
    dbHost = dbH;
    dbPort = dbPt;
    isValid = false;
}

HdbConnectionParams::HdbConnectionParams()
{
    dbUser = "hdbbrowser";
    dbPassword = "hdbbrowser";
    dbName = "hdb";

    char *hdb_server = getenv("HDB_SERVER");
    if(!hdb_server)
        isValid = false;
    else
    {
        QString hdbServer(hdb_server);
        QStringList hdbServerPort = hdbServer.split(":");
        isValid = (hdbServerPort.size() == 2);
        if(isValid)
        {
            dbHost = hdbServerPort.first();
            dbPort = hdbServerPort.last().toInt(&isValid);
        }
    }
}


TdbConnectionParams::TdbConnectionParams()
{
    dbUser = "tdbbrowser";
    dbPassword = "tdbbrowser";
    dbName = "tdb";

    char *tdb_server = getenv("HDB_SERVER");
    if(!tdb_server)
        isValid = false;
    else
    {
        QString tdbServer(tdb_server);
        QStringList tdbServerPort = tdbServer.split(":");
        isValid = (tdbServerPort.size() == 2);
        if(isValid)
        {
            dbHost = tdbServerPort.first();
            dbPort = tdbServerPort.last().toInt(&isValid);
        }
    }
}


SnapConnectionParams::SnapConnectionParams()
{
    dbUser = "snapadmin";
    dbPassword = "snapadmin";
    dbName = "snap";

    char *snap_server = getenv("HDB_SERVER");
    if(!snap_server)
        isValid = false;
    else
    {
        QString snapServer(snap_server);
        QStringList snapServerPort = snapServer.split(":");
        isValid = (snapServerPort.size() == 2);
        if(isValid)
        {
            dbHost = snapServerPort.first();
            dbPort = snapServerPort.last().toInt(&isValid);
        }
    }
}

