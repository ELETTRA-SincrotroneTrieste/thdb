#ifndef SCALARNUMBERSQUERY_H
#define SCALARNUMBERSQUERY_H

#include "dbqueryinterface.h"
#include "datainfo.h"
#include <QDateTime>
#include <QVector>
#include <QStringList>
#include <QHash>

class QString;

class ScalarNumbersQuery : public DbQueryInterface
{
    Q_OBJECT

public:

    enum State { AttIdQuery, DataQuery };

    ScalarNumbersQuery(QObject *parent = NULL);

    virtual void notify(const QSqlQuery&, const int eventId);

    virtual QString query(int id) const;

    void setWritable(bool w) { m_writable = w; }

public slots:

    unsigned int getData(int attributeId, const QString& source,
                 const QDateTime &startDateTime, const QDateTime& stopDateTime);

signals:
    void dataAvailable(const QString& source,
                       const QVector<double>& xdata,
                       const QVector<double>& yData);

    void dataSetPointAvailable(const QString& source,
                               const QVector<double> & xData,
                               const QVector<double> & yData);

protected:

private:
    QDateTime m_startDateTime, m_stopDateTime;

    QString m_source;

    int m_attributeId;

    bool m_writable;

    State m_state;

    unsigned int m_currentQueryId;

    QHash<QString, int> m_attIdHash;

    QString m_query;

    QueryDataInfoList m_dataInfoList;
};

#endif // SCALARNUMBERSQUERY_H
