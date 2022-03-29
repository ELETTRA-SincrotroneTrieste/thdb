#ifndef SWITCHDATABASEEVENT_H
#define SWITCHDATABASEEVENT_H

#include "dbqueryevent.h"
#include "dbconnectionparams.h"

class SwitchDatabaseEvent : public DbQueryEvent
{
public:
    SwitchDatabaseEvent(const DbConnectionParams & par);

    DbConnectionParams dbConnectionParams;
};

#endif // SWITCHDATABASEEVENT_H
