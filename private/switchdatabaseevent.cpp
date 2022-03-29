#include "switchdatabaseevent.h"

SwitchDatabaseEvent::SwitchDatabaseEvent(const DbConnectionParams & par)
    : DbQueryEvent(NULL, 0)
{
    dbConnectionParams = par;
    setType(DbQueryEvent::SwitchDatabase);
}
