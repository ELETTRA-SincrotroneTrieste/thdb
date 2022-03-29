#ifndef THDB_H
#define THDB_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include "thdbbase.h"

class APTData;
class AMTData;


/** \brief A facade to the Historical Database management classes.
  *
  * This class provides a simple default view of the subsystem that manages
  * data retrieval from the historical and temporary databases.
  *
  * \note
  * Every method call in this class aimed at retrieving data from the database
  * (getData, isArchived, getAptData, getAmtData, getArchivedSources) is intended
  * to be used in conjunction with its respective signal, since the underlying
  * database connection and data retrieval is organized in another thread.
  *
  */
class THdb : public THdbBase
{
    Q_OBJECT
public:

    /** \brief The constructor.
      *
      * Creates a THdb object activating queries to query the historical (or temporary)
      * database in order to get apt, amt data, the list of archived sources and
      * the data associated to a tango attribute between a start and stop date.
      *
      */
    THdb(QObject *parent = 0);

    ~THdb();


public slots:

    /** \brief Retrieves data associated to the tango attribute with name source
      *        between startDate and stopDate.
      *
      * @param source the tango attribute name, such as "test/device/1/double_scalar"
      * @param startDate the QDateTime representing the start date time
      * @param stopDate the QDateTime representing the stop date time
      *
      * Remember to connect the signal dataAvailable or dataSetPointAvailable
      * to your data display/management class in order to receive the data for
      * the source specified when it has been retrieved by the database thread.
      *
      * The dataAvailable and dataSetPointAvailable have their arguments suited
      * to be directly connected with QTango <em>TPlotLightMarker</em>.
      *
      * \par Note
      * If no data is stored between the desired start and stop date, then the database
      * is looked backwards in time for the latest available data preceding the desired
      * start date. When found,
      * the start date is set equal to the desired start date and the latest available data
      * is automatically associated to the desired start date.
      *
      * @see dataAvailable
      * @see dataSetPointAvailable
      *
      */
    void getData(const QString& source, const QDateTime& startDate, const QDateTime& stopDate);

    /** \brief asks the database whether the source is archived or not.
      *
      * This method must be used in conjunction with the isArchived signal in order
      * to be notified if the source is archived or not.
      *
      * @see isArchived
      */
    void isArchived(const QString& source);

    /** \brief gets data from apt table in the hdb or tdb database.
      *
      * @param source the tango attribute name, such as "test/device/1/double_scalar"
      *
      * \note use this method in conjunction with the aptDataReady signal, which
      * notifies you when the query execution has been performed and provides
      * the requested data.
      *
      * @see aptDataReady
      * @see APTData
      *
      * The APTData class members allow you to retrieve the parameters obtained
      * from the apt table in the historical database.
      */
    void getAptData(const QString& source);


    /** \brief gets data from amt table in the hdb or tdb database.
      *
      * @param source the tango attribute name, such as "test/device/1/double_scalar"
      *
      * \note use this method in conjunction with the amtDataReady signal, which
      * notifies you when the query execution has been performed and provides
      * the requested data.
      *
      * @see aptDataReady
      * @see AMTData
      *
      * The AMTData class members allow you to retrieve the parameters obtained
      * from the amt table in the historical database.
      */
    void getAmtData(const QString& source);

    /** \brief asks the database the list of archived sources.
      *
      * This method must be used in conjunction with the archivedSourcesListReady
      * signal which notifies you that the data has been collected and provides
      * the desired results inside its argument.
      *
      * @see archivedSourcesListReady
      */
    void getArchivedSources();

signals:

    /** \brief this signal is emitted when getArchivedSources completes.
      *
      * Connect to this signal to be notified when the list of archived attributes
      * has been retrieved from the database.
      *
      * @param sources the list of sources archived into the database as list of
      *        strings.
      */
    void archivedSourcesListReady(const QStringList &sources);

    /** \brief after the call to getData this signal carries the data for
      *        the desired source in the time interval specified in getData.
      *
      * @param source the tango attribute name which the data refers to
      * @param xData the data (timestamp) of the x axis
      * @param yData the actual data stored into the database.
      *
      * \note This signal is tailored for TPlotLightMarker/EPlotLight setData slot method.
      *
      * @see getData
      * @see dataSetPointAvailable
      */
    void dataAvailable(const QString& source, const QVector<double >& xData,
                       const QVector<double> & yData);

    /** \brief The equivalent method for dataAvailable, but here yData are the set point
      *        values for the read write attribute whose name is source.
      *
      * @see getData
      * @see dataAvailable
      *
      * Read dataAvailable documentation since the considerations are equivalent.
      */
    void dataSetPointAvailable(const QString& source, const QVector<double>& xData,
                               const QVector<double> & yData);

    /** \brief This signal carries the data retrieved from the apt table.
      *
      * To be used in conjunction with getAptData slot, this signal provides
      * the amt data for the source specified in getAptData.
      *
      * @see getAptData
      * @see APTData
      */
    void aptDataReady(const QString& source, const APTData &data);

    /** \brief This signal carries the data retrieved from the amt table.
      *
      * To be used in conjunction with getAmtData slot, this signal provides
      * the amt data for the source specified in getAmtData.
      *
      * @see getAmtData
      * @see AMTData
      */
    void amtDataReady(const QString& source, const AMTData &data);

    /** \brief this signal tells if the source is archived or not
      *
      * @param the attribute name
      * @param archived true is archived fals not archived.
      */
    void isArchived(const QString& source, bool archived);

protected:

protected slots:
    void d_getScalarData(const QString&, int, int writable, const QDateTime&, const QDateTime&);
    void d_getAptData(const QString&, int);
    void d_getAmtData(const QString& source, int attId);

private:
};

#endif // THDB_H
