#ifndef APTDATA_H
#define APTDATA_H

#include <QString>
#include <QMetaType>

/** \brief a simple class whose members store the apt data retrieved
  *        from the historical (temporary) database apt table.
  *
  * Class members are all public.
  * The apt table is described as follows:
  *
  * \code
+---------------+--------------------------+------+-----+---------+-------+
| Field         | Type                     | Null | Key | Default | Extra |
+---------------+--------------------------+------+-----+---------+-------+
| ID            | int(5) unsigned zerofill | NO   | PRI | 00000   |       |
| time          | datetime                 | YES  |     | NULL    |       |
| description   | varchar(255)             | NO   |     |         |       |
| label         | varchar(64)              | NO   |     |         |       |
| unit          | varchar(64)              | NO   |     | 1       |       |
| standard_unit | varchar(64)              | NO   |     | 1       |       |
| display_unit  | varchar(64)              | NO   |     |         |       |
| format        | varchar(64)              | NO   |     |         |       |
| min_value     | varchar(64)              | NO   |     | 0       |       |
| max_value     | varchar(64)              | NO   |     | 0       |       |
| min_alarm     | varchar(64)              | NO   |     | 0       |       |
| max_alarm     | varchar(64)              | NO   |     | 0       |       |
+---------------+--------------------------+------+-----+---------+-------+

\endcode
*
* Only some of the fields are stored in the APTData class.
*/
class APTData
{
public:
    APTData();

    /** \brief the ID field of the apt table
      */
    unsigned attId;

    /** \brief the tango attribute name whose data refers to.
      *
      * The tango attribute name, such as test/device/1/double_scalar
      */
    QString source;

    /** \brief the field time in the apt table */
    QString time;

    /** \brief the field description in the apt table */
    QString description;

    /** \brief the field label in the apt table */
    QString label;

    /** \brief the field unit in the apt table */
    QString unit;

    /** \brief the field standard_unit in the apt table */
    QString standardUnit;

    /** \brief the field display_unit in the apt table */
    QString displayUnit;

    /** \brief the field format in the apt table */
    QString format;

    /** \brief the field min_value in the apt table */
    double minValue;

    /** \brief the field max_value in the apt table */
    double maxValue;

    /** \brief the field min_alarm in the apt table */
    double minAlarm;

    /** \brief the field max_alarm in the apt table */
    double maxAlarm;
};

Q_DECLARE_METATYPE(APTData)

#endif // APTDATA_H
