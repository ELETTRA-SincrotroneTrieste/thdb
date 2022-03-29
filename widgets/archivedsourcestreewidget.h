#ifndef ARCHIVEDSOURCESTREEWIDGET_H
#define ARCHIVEDSOURCESTREEWIDGET_H

#include <QTreeWidget>
#include <thdbsingleton.h>
#include "sourcestreewidget.h"

class THdb;
class SourcesTreeWidgetPrivate;
class DbConnectionParams;

/** \brief a TreeWidget that displays the list of archived sources.
  *
  * This widget derives from QTreeWidget and provides the list of
  * tango attributes archived in the desired database, specified with
  * the Type in switchDatabase.
  */
class ArchivedSourcesTreeWidget : public SourcesTreeWidget
{
    Q_OBJECT
public:
    /** \brief The constructor, normally initialized with a parent and a
      *        THdbSingleton object, which is the single access point to
      *        the database thread in your application.
      *
      */
    explicit ArchivedSourcesTreeWidget(QWidget *parent = 0, THdb* thdb = THdbSingleton::instance());

    /** \brief specify a custom THdb object.
      *
      */
    void setTHdb(THdb* thdb);

    /** \brief returns a pointer to the currently THdb object in use
      */
    THdb* thdb() const;

    /** \brief Connects or reconnects to the historical/temporary database
      *        according to the type t and the parameters dbp
      *
      * @see THdbBase::Type
      * @see THdbBase::setType
      */
    void switchDatabase(THdbBase::Type t, const DbConnectionParams& dbp);

public slots:

    /** \brief loads the tree performing a query on the database
      */
    void load() { reload(); }

    /** \brief reloads the tree performing a query on the database
      */
    void reload();

signals:

public slots:

protected:


private:

};

#endif // ARCHIVEDSOURCESTREEWIDGET_H
