#ifndef THdb_PH_
#define THdb_PH_

#include <QtCore>
#include <QDateTime>
#include <QHash>

#include "thdbbase.h"

class THdbBase;
class DbProxy;
class DbQueryInterface;
class DbConnectionParams;

/** \brief Private member of THdbBase
  *
  * This class is not intended for programmer's use.
  */
class THdbBasePrivate
{

public:
    THdbBasePrivate(THdbBase *parent = NULL);

    ~THdbBasePrivate();

    THdbBase::Type type() const { return m_type; }

    void setType(THdbBase::Type t, const DbConnectionParams &dbCp);

    DbProxy *dbProxy() { return m_dbProxy; }

    void unregisterDbQuery(const QString& name);

    void registerDbQuery(const QString& name, DbQueryInterface *dbQuery);

    QHash <QString, DbQueryInterface *>& queryInterfaceHash() { return m_queryInterfaceHash; }

    void executeQuery(const QString& interfaceName, int id);


private:
    THdbBase * const q_ptr;

    DbProxy *m_dbProxy;

    THdbBase::Type m_type;

    QHash <QString, DbQueryInterface *> m_queryInterfaceHash;

    Q_DECLARE_PUBLIC(THdbBase);

};

#endif // THdb_PH_
