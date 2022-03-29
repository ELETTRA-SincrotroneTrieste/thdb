#ifndef DBQUERYEVENT_H
#define DBQUERYEVENT_H

#include <QString>

class DbQueryInterface;

class DbQueryEvent
{
public:

    enum Type { DatabaseQuery = 0, SwitchDatabase = 1, User = 100 };

    DbQueryEvent(DbQueryInterface *queryIf, int id);

    ~DbQueryEvent();

    DbQueryInterface *queryInterface() { return m_queryIf; }

    int id() const { return m_id; }

    Type type() const { return m_type; }

    void setType(Type t) { m_type = t; }

private:
    DbQueryInterface *m_queryIf;
    int m_id;
    Type m_type;
};

#endif // DBQUERYEVENT_H
