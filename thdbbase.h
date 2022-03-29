#ifndef THDBBASE_H
#define THDBBASE_H

#include <QObject>
#include <QVector>
#include <QDateTime>

class THdbBasePrivate;
class DbQueryInterface;
class DbConnectionParams;

class APTInfo;

/** \brief THdbBase is the base class to interact with or derive from to connect to
  *        a MySql database and execute queries in a separate thread.
  *
  * \par Description
  * THdbBase is a class which you can directly use to connect to a MySql database
  * or subclass to personalize its behaviour and create a custom interface for your
  * specific application.
  *
  *  \par
  * Create your THdbBase instance with a valid parent and then call setType or
  * setConnectionParameters to connect to a MySql database.
  * Then you can start registering your DbQueryInterface implementation with
  * registerDbQuery. You have to subclass DbQueryInterface in order to place
  * your query to the database via this framework.
  * The method dbQueryInterface returns the DbQueryInterface object with the
  * name specified when registerDbQuery was called.
  * Use unregisterDbQuery to remove a previously registered DbQueryInterface.
  *
  *
  */
class THdbBase : public QObject
{
    Q_OBJECT
public:

    /** \brief returns the version of the library, as defined in thdb.pro
      *
      */
    static QString version();

    /** \brief the database type.
      *
      * Some common types are predefined and can be use if they represent your
      * needs. Otherwise you can use User or other types.
      */
    enum Type { Unspecified = -1, Historical = 0, Temporary = 1, Snap = 2, User = 100 };

    /** \brief The constructor.
      *
      * It simply allocates a THdbBase object. As soon as this object is created,
      * the database thread is started and executing its event loop.
      * It is possible to setType to specify the connection parameters.
      *
      * \par Example
      * \code
      THdbBase *db = new THdbBase(this);
      // this class defines the connection parameters
      DbConnectionParams params("schema", "username", "password", "hostname", 3306);
      db->setType(THdbBase::User, params);
      \endcode
      *
      * @see setType
      * @see DbConnectionParams
      */
    THdbBase(QObject *parent = 0);

    /** \brief the class destructor
      *
      * If you construct a THdbBase with a valid parent, then QObject
      * destroys THdbBase for you and it is not necessary to delete it
      * manually. This destructor deletes private data.
      */
    virtual ~THdbBase();

    /** \brief Returns the database type.
      *
      * @return the database type as defined in the Type enum
      *
      * The type parameter can be useful if in your application you need to
      * switch from one database connection to another many times. The type
      * parameter can help you know which database you are connected to in
      * a certain instant.
      */
    Type type() const;

    /** \brief sets up the connection to the database specified in the DbConnectionParams
      *
      * Initiates or changes the database connection.
      * @param dbCp the DbConnectionParams describing the connection parameters (host,
               user, password, schema, database port
      * @param t the database type, not essential.
      *
      * @see setConnectionParameters
      */
    void setType(Type t, const DbConnectionParams &dbCp);

    /** \brief Changes the connection parameters without touching the type
      *
      * Changes/switches  the database connection according to the DbConnectionParams
      * object. This does not change the Type of the connection.
      *
      * @see setType
      */
    void setConnectionParameters(const DbConnectionParams &dbCp);

    /** \brief adds the dbQuery to the list of queries managed by THdb.
      *
      * The dbQuery is associated to the name.
      * You can later get the dbQuery reference by calling dbQueryInterface.
      *
      * @see unregisterDbQuery
      * @see registeredQueryInterfaces
      * @see dbQueryInterface
      */
    void registerDbQuery(const QString& name, DbQueryInterface *dbQuery);

    /** \brief removes the DbQueryInterface associated to name.
      *
      * Note: it does not delete the associated DbQueryInterface object.
      *
      * @see registerDbQuery
      * @see registeredQueryInterfaces
      * @see dbQueryInterface
      */
    void unregisterDbQuery(const QString& name);

    /** \brief returns the DbQueryInterface associated to name.
      *
      * Returns NULL if the name provided is not associated to any
      * DbQueryInterface object.
      *
      * @see registerDbQuery
      * @see registeredQueryInterfaces
      * @see unregisterDbQuery
      */
    DbQueryInterface *dbQueryInterface(const QString& name);

    /** \brief Returns the mnemonic names associated to the registered DbQueryInterfaces
      *
      * @see registerDbQuery
      * @see dbQueryInterface
      * @see unregisterDbQuery
      *
      */
    QStringList registeredQueryInterfaces() const;

protected slots:
    /** \brief an internal slot that is responsible for query execution.
      *
      * If the query with name interfaceName has been registered with
      * registerDbQuery, then it is retrieved from the internal hash
      * and a new DbQueryEvent is created and enqueued to the DbProxy
      * database thread in order to be executed.
      * <br/>
      * If interfaceName does not match any registered interfaces,
      * i.e. registeredQueryInterfaces does not contain it, the method
      * simply does nothing.
      */
    virtual void executeQuery(const QString& interfaceName, int id);

public slots:

    /** \brief This method is invoked when the database thread is started.
      *
      * The default implementation does nothing.
      */
    virtual void onThreadStarted() {};

signals:
    void databaseTypeChanged(THdbBase::Type t);

    /** \brief This signal is emitted when the database thread is started and the connection open.
      *
      * You can use this signal to be notified when it is possible to make queries.
      */
    void databaseOpen();

    /** \brief This signal is emitted when the database thread is started.
      *
      */
    void dbThreadStarted();

    /** \brief This signal is emitted when the database thread finishes.
      *
      */
    void dbThreadFinished();

    /** \brief emitted just right before calling exec() method
      *
      * You can use this signal to evidentiate in your application that the
      * query is being performed in the background, i.e. in the database
      * thread.
      */
    void querySent();

    /** \brief emitted right after exec() returns. The result is
      *        containtd into result variable.
      *
      * @param result true if the QSqlQuery::exec() method is successful.
      */
    void queryExecuted(bool result);

    /** \brief this signal is emitted if exec() returns false.
      *
      * @param lastErrorMessage the associated error message.
      * @param origin a description of the origin of the error
      *
      * @see querySent
      * @see queryExecuted
      */
    void databaseError(const QString& origin, const QString& lastErrorMessage);

    /** \brief another signal containing an informational message
      *
      * This signal is provided for convenience, should it fit your
      * signal/slot connection needs better than the above signal.
      *
      * @see databaseError
      * @see querySent
      * @see queryExecuted
      */
    void dbConnectionInfo(const QString &info);

protected:

private:
    Q_DECLARE_PRIVATE(THdbBase);
    THdbBasePrivate * d_ptr;

};

#endif // THDB_H
