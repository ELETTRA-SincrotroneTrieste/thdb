#ifndef QUERYRESULTEVENT_H
#define QUERYRESULTEVENT_H

#include <QEvent>
#include <QSqlQuery>

#define QResultEvent (QEvent::Type) (QEvent::User + (QEvent::Type) 43)

class QueryResultEvent : public QEvent
{
public:

    explicit QueryResultEvent(const QSqlQuery &query, int eventId);

    QSqlQuery result() const { return m_result; }

    int eventId() const { return m_eventId; }

private:
    QSqlQuery m_result;
    int m_eventId;
};

#endif // QUERYRESULTEVENT_H
