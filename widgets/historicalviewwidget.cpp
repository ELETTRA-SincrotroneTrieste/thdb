#include "historicalviewwidget.h"
#include "historicalviewwidgetprivate.h"
#include <macros.h>

HistoricalViewWidget::~HistoricalViewWidget()
{
    printf("\e[1;31m~HistoricalViewWidget() - \e[0m destroyed\n");
}

HistoricalViewWidget::HistoricalViewWidget(QWidget *parent) : QStackedWidget(parent)
{
    printf("HistoricalViewWidget::HistoricalViewWidgetcount() %d\n", count());
    d_ptr = new HistoricalViewWidgetPrivate(this);
}

void HistoricalViewWidget::registerWidget(QWidget *w, DataType t)
{
    int idx = addWidget(w);
    printf("\e[1;34m adding \"%s\" class \"\e[1;36m%s\e[0m\" to index %d\e[0m\n", qstoc(w->objectName()),
           w->metaObject()->className(), idx);
    d_ptr->addTypeIndex(t, idx);
}

void HistoricalViewWidget::switchWidget(DataType t)
{
    int idx = d_ptr->index(t);
    if(idx > -1)
    {
        printf("\e[1;32m widget switch requested to type %d\e[0m\n", t);
        setCurrentIndex(idx);
    }
    else
        perr("no widget registered for type %d", t);
}

QWidget *HistoricalViewWidget::widget(DataType t)
{
    int idx = d_ptr->index(t);
    if(idx > -1)
        return QStackedWidget::widget(idx);
    return NULL;
}

