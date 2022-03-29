#ifndef SOURCESTREEWIDGET_P_H
#define SOURCESTREEWIDGET_P_H

#include <thdb.h>

class SourcesTreeWidget;

class SourcesTreeWidgetPrivate
{
public:
    SourcesTreeWidgetPrivate(SourcesTreeWidget *tree);

    THdb *thdb;

private:
    Q_DECLARE_PUBLIC(SourcesTreeWidget);

    SourcesTreeWidget* q_ptr;
};

#endif // ARCHIVEDSOURCESTREEWIDGET_P_H
