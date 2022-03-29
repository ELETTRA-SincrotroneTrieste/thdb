#ifndef AMTDATA_H
#define AMTDATA_H

#include <QString>
#include <QMetaType>
#include <QDateTime>
#include <QList>

/** \brief a simple class whose members store the amt data retrieved
  *        from the historical (temporary) database amt table.
  *
  * Class members are all public.
  * The amt table is described as follows:
  * \code
  * +-------------------+-------------------------------+------+-----+---------+-------+
| Field             | Type                          | Null | Key | Default | Extra |
+-------------------+-------------------------------+------+-----+---------+-------+
| ID                | smallint(5) unsigned zerofill | NO   | MUL | 00000   |       |
| archiver          | varchar(255)                  | NO   |     |         |       |
| start_date        | datetime                      | YES  |     | NULL    |       |
| stop_date         | datetime                      | YES  |     | NULL    |       |
| per_mod           | int(1)                        | NO   |     | 0       |       |
| per_per_mod       | int(5)                        | YES  |     | NULL    |       |
| abs_mod           | int(1)                        | NO   |     | 0       |       |
| per_abs_mod       | int(5)                        | YES  |     | NULL    |       |
| dec_del_abs_mod   | double                        | YES  |     | NULL    |       |
| gro_del_abs_mod   | double                        | YES  |     | NULL    |       |
| rel_mod           | int(1)                        | NO   |     | 0       |       |
| per_rel_mod       | int(5)                        | YES  |     | NULL    |       |
| n_percent_rel_mod | double                        | YES  |     | NULL    |       |
| p_percent_rel_mod | double                        | YES  |     | NULL    |       |
| thr_mod           | int(1)                        | NO   |     | 0       |       |
| per_thr_mod       | int(5)                        | YES  |     | NULL    |       |
| min_val_thr_mod   | double                        | YES  |     | NULL    |       |
| max_val_thr_mod   | double                        | YES  |     | NULL    |       |
| cal_mod           | int(1)                        | NO   |     | 0       |       |
| per_cal_mod       | int(5)                        | YES  |     | NULL    |       |
| val_cal_mod       | int(3)                        | YES  |     | NULL    |       |
| type_cal_mod      | int(2)                        | YES  |     | NULL    |       |
| algo_cal_mod      | varchar(20)                   | YES  |     | NULL    |       |
| dif_mod           | int(1)                        | NO   |     | 0       |       |
| per_dif_mod       | int(5)                        | YES  |     | NULL    |       |
| ext_mod           | int(1)                        | NO   |     | 0       |       |
| refresh_mode      | tinyint(4)                    | YES  |     | 0       |       |
+-------------------+-------------------------------+------+-----+---------+-------+
  * \endcode
  * but the AMTData class simply returns some of the items in the table.
  */
class AMTData
{
public:
    /** \brief the class constructor. It does nothing
     *
     */
    AMTData();

    /** \brief the attribute ID as stored in the ID column of the amt table
      *        in the historical or temporary database.
      */
    unsigned attId;

    /** \brief stores the archiver field in the amt table of the hdb database
      */
    QString archiver;

    /** \brief stores the start_date field of the amt table.
      */
    QDateTime startDate;

    /** \brief stores the stop_date field of the amt table.
      */
    QDateTime stopDate;

    /** \brief stores the per_mod of the amt table
      */
    unsigned perMod;

    /** \brief stores the per_per_mod of the amt table
      */
    unsigned perPerMod;
};

Q_DECLARE_METATYPE(AMTData)

#endif // AMTDATA_H
