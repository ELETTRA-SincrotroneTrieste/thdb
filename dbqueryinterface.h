#ifndef DBQUERYINTERFACE_H
#define DBQUERYINTERFACE_H

#include <QObject>
#include <QString>

class QSqlQuery;
class DbQueryEvent;
class THdbBase;


/** \brief Derive from this class to perform threaded queries on a database.
  *
  * <h3>Description</h3>
  * <p>
  * This class is a QObject and it is used to perform queries on the database.
  * Queries are executed on a different thread, so that your application will
  * remain fully responsive even during time consuming queries.
  * In order to use DbQueryInterface, you must derive it and provide the
  * implementation of the two methods
  * <ul>
  * <li>void notify(const QSqlQuery&, const int eventId)</li>
  * <li>QString query(unsigned id) const</li>
  * </ul>
  * They are two pure virtual methods.
  * </p>
  * <h3>The query method</h3>
  * <p>The query method is invoked when the Database thread needs to
  * know the query string it has to execute. Simply return the query
  * as a string, such as <br/><cite>"SELECT * FROM context"</cite><br/>
  * </p>
  * <h3>The notify method</h3>
  * <p>
  * This callback is invoked <strong>in the main thread </strong> after that
  * the data is retrieved from the database, if correctly retrieved.
  * In any case, the QSqlQuery passed in as parameter will contain errors,
  * if they happened.
  * Here you get the result of the query just executed in the database thread,
  * you have to process it as if you did directly use QSqlQuery from Qt SQL module.
  * notify is invoked with an integer as eventId, the sami integer that was passed
  * to the execute method.
  * </p>
  * <h3>The execute method</h3>
  * <p>
  * invoke the execute method with  an optional integer aimed at identifying the
  * result in the notify method. If you need to call the method more than once
  * and then recognize among the results the originating query, then it is safe
  * to pass an integer to the execute method.
  * <br/> Otherwise, if you are sure that, in your specific application, each
  * query is executed after the previous has completely ended (such as you click
  * a button, you wait for results to appear in your application, and then perform
  * another query), you may omit the id in  the execute method.
  * </p>
  *
  * <h3>Signals and slots in DbQueryInterface</h3>
  * <p>This class is a QObject so you are free to define signals and
  * slots in order to make it interact with other classes.
  * </p>
  *
  */
class DbQueryInterface : public QObject
{
    Q_OBJECT
public:
    /** \brief The constructor.
      *
      * Note: this inherits QObject.
      */
    explicit DbQueryInterface(QObject *parent = NULL);

    /** \brief Pure virtual method that must be implemented in your subclass.
      *
      * This is invoked in the main thread.
      * @param queryResult the QSqlQuery returned by the QSqlQuery::exec() method.
      *        It contains the data (see Qt QSqlQuery documentation).
      *
      * @param eventId the same integer previously passed to the execute method
      *        that allows to recognize the query originating the result stored
      *        in queryResult.
      */
    virtual void notify(const QSqlQuery& queryResult, const int eventId) = 0;

    /** \brief Triggers query execution on the Database thread.
      *
      * when you call this method, the query execution is triggered and the
      * database thread will execute it as soon as possible. When done, the
      * notify callback is invoked (always in the main thread), so that the
      * results can be parsed and used.
      * @see notify
      *
      * @param id if you need to recognize inside the notify method the query that
      *        originated the result in notify, then pass a unique id to this
      *        method. The id is propagated through all the Database thread calls
      *        to guarantee it univokely identifies the query.
      *
      * When you call execute, you must be ready to provide a QString representing
      * a valid query inside the query() method below.
      */
    virtual void execute(int id = -1);

    /** \brief Returns the QString representing the query to be executed.
      *
      * @return QString a string representing the desired query.
      *
      * This is pure virtual and must be implemented.
      * @param id You can store a map of queries such as a QMap<int, QString>
      *        in your application and then return the correct query according
      *        to the id.
      *        The id is the same that the framework propagates from the execute
      *        to the notify, passing through this query() method.
      */
    virtual QString query(int id) const = 0;

    /** \brief returns the interface name.
      *
      * You needn't normally use this method
      */
    QString interfaceName() const { return m_interfaceName; }

    /** \brief changes the interface name
      *
      * You needn't normally use this method
      *
      */
    void setInterfaceName(const QString& name) { m_interfaceName = name; }

    void setId(const int i) { m_id = i; }

protected:

    /** \brief Internally used, detects if e is QueryResultEvent and calls notify
      *        callback.
      *
      * If the event type is QResultEvent, then the notify method is invoked
      * on your subclass implementation. QueryResultEvent knows about the query
      * result and the event id. These two parameters are passed to your notify
      * implementation.
      */
    bool event(QEvent *e);

signals:
    /** \brief signal connected to THdbBase to trigger query execution.
      *
      * This signal is connected inside THdbBase::registerDbQuery to
      * trigger the query execution when execute() method is invoked.
      *
      * You should not need use this signal in your application, unless
      * you want to be notified when execute() is called.
      *
      * @param interfaceName the name of the interface as specified in
      *        THdbBase::registerDbQuery
      * @param id the integer specified when calling execute.
      *
      */
    void executeSignal(const QString& interfaceName, int id);

public slots:

private:
    QString m_interfaceName;
    int m_id;

};

#endif // DBQUERYINTERFACE_H
