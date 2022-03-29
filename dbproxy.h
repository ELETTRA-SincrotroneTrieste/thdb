#ifndef DBPROXY_H
#define DBPROXY_H

#include <QThread>
#include <QReadWriteLock>
#include <QtSql/QSqlDatabase>
#include <QDateTime>
#include <QQueue>
#include "dbconnectionparams.h"

#include <QEvent>
#include <QCoreApplication>

class APTInfo;
class DbQueryEvent;

/** \brief The database proxy. Connects to the database and executes queries
  *        in a separate thread. You better not use it directly, instead look
  *        at THdbBase and DbQueryInterface classes.
  *
  * Use this class only if you need very specific behaviour. Otherwise, look
  * at THdbBase and DbQueryInterface classes, which are the essential classes
  * to build a database connection and submit any type of query.
  */
class DbProxy : public QThread
{
    Q_OBJECT

public:

    /** \brief Returns the instance associated to the Database Thread.
      *
      * Should you ever directly use this thread, remember to wait()
      * on it when opportune.
      *
      * On first instance invocation, a new thread is started and it executes
      * its event loop waiting for DbQueryEvent events.
      * <p>
      * <strong>Note</strong>: you shouldn't need to use this class directly.
      * Instead see THdbBase, THdb, DbQueryInterface classes.
      * </p>
      * <p>
      * The thread exits when QCoreApplication emits the <strong>aboutToQuit</strong>
      * signal, so that you needn't worry about the thread exit. Instead, wait() must
      * be called if you do not rely on THdbBase class in your application.
      * </p>
      */
    static DbProxy *instance(QObject *parent = NULL);

    /** \brief returns true if an error happened during last operation on the database.
      *
      * @return true an error is present. Get it with lastErrorMessage
      *
      * @see lastErrorMessage
      */
    bool error() const { return m_dbError; }

    /** \brief returns the last <strong>connection</strong> error message.
      *
      * @return the last <strong>connection</strong> error message.
      *
      * <h3>Note</h3><p>
      * This does not contain the last query execution error message. <br/>
      * databaseError and connectionInfo <strong>signals</strong> have to be
      * used to receive error notifications on query exec().
      *
      * </p>
      * <h3>Another note</h3>
      * <p>THdbBasePrivate manages signals emitted by this class.
      * @see THdbBasePrivate constructor
      * </p>
      *
      */
    QString lastErrorMessage() const { return m_lastErrorMessage; }

    /** \brief adds a DbQueryEvent on the event queue and then asks the
      *        thread to process the queue.
      *
      * This is invoked by THdbBasePrivate::executeQuery in the main thread.
      * At the end, after enqueueing the event, the process request signal
      * is emitted to tell the thread to process the event queue.
      */
    void addQueryEvent(DbQueryEvent *dbqe);

    /** \brief switches to the database specified by the argument
      *
      * Enqueues a SwitchDatabaseEvent on the event queue so that
      * the current connection is closed and a new one is opened
      * according to the parameters.
      *
      * @see DbConnectionParams
      */
    void setDatabase(const DbConnectionParams& dbconPar);

    /** \brief returns true if the database is open.
      *
      */
    bool databaseIsOpen() const { return m_sqlDb.isOpen(); }


protected:

    /** \brief the constructor of the Database thread, called proxy.
      *
      *
      * Starts the thread after connecting the QCoreApplication::aboutToQuit signal
      * to the appAboutToQuit slot.
      *
      */
    DbProxy(QObject *parent);

    virtual ~DbProxy();

    /** \brief the run method of the Database thread.
      *
      *
      * <ul>
      * <li>initiates the QSqlDatabase by calling addDatabase with QMYSQL as parameter;</li>
      * <li>creates an exit loop timer to exit the thread when
      *    QCoreApplication::instance() emits the aboutToQuit signal;</li>
      * <li>creates a processTimer to empty the queue just when the thread starts
      *     running (events might have been accumulated);</li>
      * <li>creates a queued connection to manage query and other event execution.</li>
      * <li>Calls exec to start the event loop of the thread.</li>
      *
      * </ul>
      */
    virtual void run();

    QSqlDatabase m_sqlDb;

    bool m_dbError;

    QString m_lastErrorMessage;

    QString m_host, m_user, m_pass, m_schema;
    unsigned short m_port;

signals:
    void exitLoop();

    void processRequest();

    void error(const QString& origin, const QString& message);

    void databaseOpen();

    /** \brief emitted just right before calling exec() method
      *
      */
    void querySent();

    /** \brief emitted right after exec() returns. The result is
      *        containtd into result variable.
      *
      */
    void queryExecuted(bool result);

    /** \brief this signal is emitted if exec() returns false.
      *
      * @param lastErrorMessage the associated error message.
      * @param origin the origin of the error.
      *
      * Use this signal if you have an error reporting framework suitable
      * to accept an origin and a message parameter. If you only need a
      * single connection information message, please see connectionInfo
      *
      * @see connectionInfo
      */
    void databaseError(const QString& origin, const QString& lastErrorMessage);

    /** \brief This signal is emitted whenever something changes in the connection
      *   status.
      *
      * This signal is emitted on database connection (be it successful or not) and
      * when there is an error executing a query.
      *
      * @see databaseError
      *
      */
    void connectionInfo(const QString& info);

private slots:

    void appAboutToQuit();

    void process();

    void close();

private:

    int m_queryCnt;

    QQueue<DbQueryEvent *> m_eventQueue;

    QReadWriteLock m_rwLock;

    DbConnectionParams m_dbConnectionParams;

    static DbProxy* _instance;

    void m_switchDatabase();

};

#endif // DBPROXY_H
