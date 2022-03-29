#include "archivedsourcestreewidget.h"
#include <dbconnectionparams.h>
#include "sourcestreewidget_p.h"
#include <QTreeWidgetItem>
#include <QMimeData>
#include <QDropEvent>
#include <macros.h>

ArchivedSourcesTreeWidget::ArchivedSourcesTreeWidget(QWidget *parent, THdb* thdb) :
    SourcesTreeWidget(parent)
{
    d_ptr->thdb = thdb;

    connect(d_ptr->thdb, SIGNAL(archivedSourcesListReady(QStringList)),
            this, SLOT(updateSourcesList(QStringList)));
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QTreeView::DragDrop);
}

void ArchivedSourcesTreeWidget::switchDatabase(THdbBase::Type t, const DbConnectionParams& dbp)
{
    /* change database connection */
    d_ptr->thdb->setType(t, dbp);
    QString headerTxt = headerItem()->text(0);
    QString s;
    (t == THdbBase::Historical) ? (s = "HDB") : (s = "TDB");
    if(headerTxt.contains(QRegExp("[H|T]DB")))
        headerTxt.replace(QRegExp("[H|T]DB"), s);
    else
        headerTxt += "[" + s + "]";
    headerItem()->setText(0, headerTxt);
    reload();
}

void ArchivedSourcesTreeWidget::reload()
{
    clear();
    d_ptr->thdb->getArchivedSources();
}
