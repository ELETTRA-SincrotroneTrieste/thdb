/** \mainpage THdb Qt and MySQL interface to a database.
 *
 * \brief This class gives access to the database and allows registering DbQueryInterface
 *   objects, which are the means to query the database.
 *
 * <p>
 * THdb is a library based on Qt that provides a programmer interface
 * to easily execute queries on a separate thread and notify the user
 * when the query execution terminates.
 *
 * The main classes to keep in mind when using the library are
 * <ul>
 * <li>THdbBase: manages database connection and queries;</li>
 * <li>DbQueryInterface: an interface that <em>must be reimplemented</em>
 *     in order to define the desired query and to be notified when
 *     the query execution ends and data are ready.</li>
 * <li>DbConnectionParams: a class that contains the relevant parameters to
 *     open and setup a valid connection to a MySql database.</li>
 * </ul>
 *
 * \note These three classes should be enough for you to write an application
 *       to connect to a MySql database and to submit any type of query to
 *       it.
 *
 * \note
 * Under the directory <strong>example/thdbDemo</strong> you'll find a
 * simple example of an application that executes a query and shows the results
 * on a QTableWidget. You may want to have a look at that application first.
 * </p>
 *
 * <h3>Historical Database (HDB) tailored classes</h3>
 * <p>
 * The library provides a specific implementation of THdbBase which is called
 * THdb, to which the reader is sent if interested to interact with that specific
 * database.
 *
 * @see THdb
 *
 * </p>
 *
 * <h3>Framework description</h3>
 * <p>
 * The main feature is that <strong>the query is executed in another thread</strong>.
 * When data is ready, the DbQueryInterface::notify() method is called and actually
 * represents a callback. You must write your own notify() method in order to
 * receive and organize the data collected from the database.
 * </p>
 * <p>
 *  If subsequent queries are sent to the database via the DbQueryInterface::execute
 * method, they are <strong>queued</strong>, so that the <strong>execution order is
 * guaranteed</strong>. Even a database switch, actuated by means of the THdbBase::setType
 * method, is enqueued just like any other database request, so that if you send
 * subsequently
 *
 * \code
    query1->execute();
    query2->execute();
    thdbBaseObject->setType(THdbBase::User, DbConnectionParams(...));
    query3->execute();
  \endcode
 *
 * then query1 and query2 are executed, in order, then another connection is
 * opened to the database specified in DbConnectionParams and query3 is sent
 * to the new database.
 * </p>
 *
 * <h2>Demo example</h2>
 * <p> Under the directory <strong>example/thdbDemo</strong> you'll find a
 * simple example of an application that executes a query and shows the results
 * on a QTableWidget. The query is executed in the database thread and  the
 * table is populated on data available notification.
 * Database messages are used to display the success or failure of each
 * phase of the database interaction. You may want to have a look at the example
 * as starting point.
 * </p>
 *
 * <h3>Example</h3>
 * <p>
 * As an example, suppose we have to connect to a database whose schema name is
 * <em>snap</em> and we have to retrieve a list of <em>contexts</em> by
 * means of a simple query.
 *
 * \par Implementation of a DbQueryInterface class
 *
 * We must derive from DbQueryInterface and implement the two pure virtual methods
 * <ul><li>QString query(unsigned id) const;</li>
 * <li>void notify(const QSqlQuery&, const int eventId);</li>
 * </ul>
 * The query() method is called by the database thread when it's time to execute the
 * query, normally when the DbQueryInterface::execute() method is invoked.
 * <br/>
 * The notify() method is invoked when the query is executed and the query result
 * is passed as parameter, together with an <strong>eventId</strong> integer
 * identifying the query id with which the execute was called.
 * <br/>
 * You might not need to specify a query id when calling execute(), and in fact
 * in that case it defaults to -1. Yet, since the query is executed in another thread
 * and you may want to queue more than one query at once, on result notification
 * you may be interested in recognizing the query your result comes from.
 * </p>
 *
 *
 *
 * \code
 #include <DbQueryInterface>
class CtxListQuery : public DbQueryInterface
{
    Q_OBJECT
public:
    // the class constructor
    CtxListQuery(QObject *parent = NULL);

    // the notify method, must be reimplemented because pure virtual
    virtual void notify(const QSqlQuery&, const int eventId);
    // this is normally not reimplemented, but you may want to do something
    // before executing the query
    virtual void execute(int id = -1);
    // simply must return the query as string.
    virtual QString query(unsigned id) const;

signals:
    // this signal is emitted by my CtxListQuery class when data is ready.
    // Inside notify(), query results are extracted and placed in a list
    // of ContextData objects.
    // It is not relevant for this purpose to know what ContextData is like,
    // anyway the class definition is reported below.
    void listReady(const QList<ContextData>&);
};

// class ContextData: contains a bunch of strings and an integer
#include <QString>

class ContextData
{
public:
    ContextData();

    short id;
    QString time;
    QString name, author, reason, description;

};

\endcode

<h3>Implementing the CtxListQuery class</h3>
<p>
The implementation of the CtxListQuery class is very simple

\par CtxListQuery implementation
\code

#include "ctxlistquery.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

CtxListQuery::CtxListQuery(QObject *parent) : DbQueryInterface(parent)
{

}

void CtxListQuery::notify(const QSqlQuery& constQuery, const int eventId)
{
    // list of data to fill in
    QList<ContextData> ctxDataList;
    QSqlQuery q(constQuery);
    // in this case the query contains 6 fields
    while(q.next() && q.record().count() == 6)
    {
        ContextData cd;
        // use QSqlRecord and QSqlQuery methods to extract
        // data from the query
        cd.id = q.value(0).toInt();
        cd.time = q.value(1).toString();
        cd.name = q.value(2).toString();
        cd.author = q.value(3).toString();
        cd.reason = q.value(4).toString();
        cd.description = q.value(5).toString();
        ctxDataList << cd;
    }
    // this was our signal to notify the result
    emit listReady(ctxDataList);
}

void CtxListQuery::execute(int id)
{
    // simply call base implementation in this case
    DbQueryInterface::execute(id);
}

QString CtxListQuery::query(unsigned id) const
{
    // create our sql query and return it as a QString
    QString q = "SELECT * from context";
    return q;
}

\endcode

</p>

<h3>Connecting to the database and executing the query </h3>
<p>

A THdbBase object (or a derived class) must be created. Then you must
register your CtxListQuery via the THdbBase::registerDbQuery method

\par Database connection and query execution
\code
    CtxListQuery *ctxListQuery = new CtxListQuery(this);

    // use the listReadySignal to obtain query results when the
    // query has been executed
    connect(ctxListQuery, SIGNAL(listReady(const QList<ContextData>&)), this,
        SLOT(contextListReady(const QList<ContextData>&)));

    THdbBase *thdbBase = new THdbBase(this);

    // registerDbQuery is called with a string and the object to register
    // the string is useful to obtain the ctxListQuery back with the
    // THdbBase::dbQueryInterface method
    thdbBase->registerDbQuery("CtxListQuery", ctxListQuery);

    // now you are ready to execute the query by calling the execute method
    // on the ctxListQuery. You can also connect the clicked signal of a push
    // button to the execute slot of the ctxListQuery object, like this
    connect(pushButtonExecQuery, SIGNAL(clicked()), ctxListQuery, SLOT(execute()));

 \endcode

</p>
  *
  */
