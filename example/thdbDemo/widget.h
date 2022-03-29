#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <DbQueryInterface>

namespace Ui {
    class Widget;
}

class MyQuery : public DbQueryInterface
{
    Q_OBJECT
public:
    MyQuery(QObject *parent) : DbQueryInterface(parent) {}

    /* The following two must be implemented cause pure virtual */

    /** \brief This method extracts data from the query result.
      *
      * Detect if an error happened, then extract the result contained
      * in queryResult. Emit message signal to notify the success or
      * failure. Emit header and rowReady signals to allow the QTableWidget
      * to show the query results.
      */
    void notify(const QSqlQuery &queryResult, const int eventId);

    QString query(int /* id */ /* unused */) const { return m_query; }

public slots:
    void setQuery(const QString &q) { m_query = q; }

signals:
    void header(const QStringList &items);
    void rowReady(const QStringList& fields);
    void querySuccessful(const QString& query, bool successful);
    void message(const QString& message);

private:
    QString m_query;

};

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void openDbConnection();

    void setTreeHeader(const QStringList& items);

    void addTreeItem(const QStringList &row);

    void executeQuery();

    void querySuccessful(const QString& query, bool success);

    /** \brief connected to Database thread querySent signal,
      *        to indicate that the thread called exec()
      */
    void querySent();

private:
    Ui::Widget *ui;

    QString getHistorySettingsKey();

    void loadLastConnectionParams();

    void storeLastConnectionParams();
};

#endif // WIDGET_H
