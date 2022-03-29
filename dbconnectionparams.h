#ifndef DBCONNECTIONPARAMS_H
#define DBCONNECTIONPARAMS_H

#include <QString>

/** \brief this class contains the parameters that define a connection to a MySql
  *        database.
  *
  * Use this class to create a database connection or to switch an already active
  * connection to another one with different parameters.
  *
  * This class is used in conjunction with THdbBase::setType or
  * THdbBase::setConnectionParameters.
  *
  * @see THdbBase::setConnectionParameters
  * @see THdbBase::setType
  * @see THdbBase
  */
class DbConnectionParams
{
public:

    /** \brief the constructor, which takes as input arguments all the
      *        necessary parameters used to connect to a MySql database.
      *
      * @param dbName the database name, or schema, such as "tdb", "hdb", "snap"...
      * @param dbUser the user name to use to connect to the database
      * @param dbPassword the password to use to connect to the database
      * @param dbHost the name of the host running the database, such as
      *        "hokuto", "hokuto.elettra.trieste.it"
      * @param dbPort the tcp port on which the database server is listening
      *        to. The  default value is 3306.
      *
      * You can also construct a DbConnectionParams object with an empty
      * constructor and then set the single parameters of the connection,
      * which are simple public variables.
      */
    DbConnectionParams(const QString& dbName = QString(),
                       const QString& dbUser = QString(),
                       const QString& dbPassword = QString(),
                       const QString& dbHost = QString(),
                       unsigned short dbPort = 3306);

    QString dbName, dbUser, dbPassword, dbHost;
    unsigned short int dbPort;

    bool isValid;
};

/** \brief DbConnectionParams for hdbbrowser user of the hdb database.
  *
  * \note the HDB_SERVER environment variable is requested for this to
  *       work and it must be host:PORT
  *
  */
class HdbConnectionParams : public DbConnectionParams
{
public:
    HdbConnectionParams();
};

/** \brief DbConnectionParams for tdbbrowser user of the tdb database.
  *
  * \note the HDB_SERVER environment variable is requested for this to
  *       work and it must be host:PORT
  */
class TdbConnectionParams : public DbConnectionParams
{
public:
    TdbConnectionParams();
};

/** \brief DbConnectionParams for snapadmin user of the snap database.
  *
  * \note the HDB_SERVER environment variable is requested for this to
  *       work and it must be host:PORT
  */
class SnapConnectionParams : public DbConnectionParams
{
public:
    SnapConnectionParams();
};


#endif // DBCONNECTIONPARAMS_H
