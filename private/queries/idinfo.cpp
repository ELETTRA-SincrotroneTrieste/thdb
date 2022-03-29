#include "idinfo.h"
#include <QtDebug>
#include <QThread>

IdInfo::IdInfo(unsigned query_id, const QString& src, Reason r) : QueryDataInfo(query_id)
{
    source = src;
    m_reason = r;
}

QString IdInfo::query() const
{
    return QString("SELECT full_name,ID,writable FROM adt WHERE full_name='%1'").arg(source);
}
