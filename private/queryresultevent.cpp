#include "queryresultevent.h"

QueryResultEvent::QueryResultEvent(const QSqlQuery &query, int eventId) :
    QEvent(QResultEvent)
{
    m_result = query;
    m_eventId = eventId;
}
