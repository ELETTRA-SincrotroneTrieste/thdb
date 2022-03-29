#include "thdbplot.h"
#include "thdbplotprivate.h"
#include <hdbplotadapter.h> /* qtango4 dependency */
#include <tdbplotadapter.h>
#include <macros.h>
#include <QtDebug>

THdbPlot::~THdbPlot()
{
    printf("\e[1;31mTHdbPlot::~THdbPlot() \e[0m invoked\n");
    foreach(DbPlotAdapterInterface *dbai, findChildren<DbPlotAdapterInterface *>())
    {
        printf("\e[0;31m - deleting dbPlotAdapterInterface %s\e[0m\n", qstoc(dbai->objectName()));
        delete dbai;
    }
}

THdbPlot::THdbPlot(QWidget *parent) : TPlotLightMarker(parent)
{
    setObjectName("THdbPlot");
    d_ptr = new THdbPlotPrivate(this);
    d_ptr->currentType = THdbBase::Unspecified;
    setHdbActionEnabled(false);
}

void THdbPlot::setTHdb(THdb* t)
{
    DbPlotAdapterInterface *dbai = NULL;
    TdbPlotAdapter *tdbpa = NULL;
    if(d_ptr->thdb)
        d_ptr->thdb->disconnect(this);
    d_ptr->thdb = t;
    d_ptr->currentAdapterInterface = getAdapterIf();
    connect(d_ptr->thdb, SIGNAL(databaseTypeChanged(THdbBase::Type)), this, SLOT(databaseChanged(THdbBase::Type)));
}

void THdbPlot::setType(THdbBase::Type t)
{
    if(d_ptr->thdb)
    {
        if(t != d_ptr->thdb->type()) /* remove current historical curves */
        {
            DbPlotAdapterInterface *dbai = getAdapterIf();
            if(dbai)
            {
                printf("\e[1;31mcalling deleteHistoricalCurves() by setType\e[0m\n");
                dbai->deleteHistoricalCurves();
            }
        }
        else
            printf("\e[1;31msame tye in setType()\e[0m\n");
        if(t == THdbBase::Historical)
            d_ptr->thdb->setType(t, HdbConnectionParams());
        else
            d_ptr->thdb->setType(t, TdbConnectionParams());
    }
}

THdbBase::Type THdbPlot::type() const
{
    if(d_ptr->thdb)
        return d_ptr->thdb->type();
    return THdbBase::Unspecified;
}

THdb* THdbPlot::thdb() const
{
    return d_ptr->thdb;
}

void THdbPlot::databaseChanged(THdbBase::Type t)
{
    printf("\e[0;33m DATABASE CHANGED \e[0m\n");
    DbPlotAdapterInterface *dbpai = getAdapterIf();
    if(dbpai)
        dbpai->deleteHistoricalCurves();
}

void THdbPlot::update(const QStringList& sources)
{
    d_ptr->sources =  sources;

    if(d_ptr->liveMode)
    {
        DbPlotAdapterInterface *dbpai;
        setSourcesList(d_ptr->sources);
        if((dbpai = getAdapterIf()) != NULL)
        {
            printf("\e[1;32mUNIFORMIZING CURVE STYLES\e[0m\n");
            dbpai->uniformizeCurveStyles();
        }
        else
            printf("\e[1;31mNOT UNIFORMIZING NULL dbpai!\e[0m\n");
    }
    else
    {
        QStringList currentSources = sourcesList();
        unsetSources();
        foreach(QString s, currentSources)
        {
            printf("\e[1;31m deleting %s\e[0m\n", qstoc(s));
            removeCurve(s);
        }
    }

}

void THdbPlot::setLiveModeEnabled(bool live)
{
    d_ptr->liveMode = live;
}

bool THdbPlot::liveModeEnabled() const
{
    return d_ptr->liveMode;
}

void THdbPlot::requestWidgetSwitch()
{
    qDebug() << __FUNCTION__;
    if(parent()->inherits("HistoricalViewWidget"))
    {
        printf("\e[1;31m no requestWidgetSwitch implementation!!!\e[0m\n");
        // qobject_cast<HistoricalViewWidget *>(parent())->switchWidget(HistoricalViewWidget::NumberScalar);
        qDebug() << __FUNCTION__ << "parent inherits HistoricalViewWidget: switch requested";
    }
}

void THdbPlot::m_installAdapterInterface(THdbBase::Type t)
{
    DbPlotAdapterInterface *dbai = NULL;
    dbai = findChild<DbPlotAdapterInterface *>();
    if(dbai)
    {
        printf("\e[1;31mremoving previously installed adapter interface \"%s\"\e[0m\n",
               qstoc(dbai->objectName()));
        dbai->deleteHistoricalCurves();
        delete dbai;
        dbai = NULL;
    }
    if(t == THdbBase::Historical)
    {
        dbai = new HdbPlotAdapter(this);
        dbai->setObjectName("HdbPlotAdapter");
    }
    else if(t == THdbBase::Temporary)
    {
        dbai = new TdbPlotAdapter(this);
        dbai->setObjectName("TdbPlotAdapter");
    }
    if(dbai)
    {
        connect(d_ptr->thdb, SIGNAL(dataAvailable(QString,QVector<double>,QVector<double>)),
                dbai, SLOT(setData(QString,QVector<double>,QVector<double>)));
        connect(d_ptr->thdb, SIGNAL(dataSetPointAvailable(QString,QVector<double>,QVector<double>)),
                dbai, SLOT(setSetPointData(QString,QVector<double>,QVector<double>)));
        if(parent()->inherits("HistoricalViewWidget"))
            connect(dbai, SIGNAL(dataUpdated()), this, SLOT(requestWidgetSwitch()));
        connect(dbai, SIGNAL(dataUpdated()), this, SIGNAL(historicalDataUpdated()));
    }
}

DbPlotAdapterInterface *THdbPlot::getAdapterIf()
{
    DbPlotAdapterInterface *dbai = NULL;

    if(!d_ptr->thdb)
        return dbai;

    THdbBase::Type t = d_ptr->thdb->type();
    if(t == THdbBase::Historical)
    {
        dbai = findChild<HdbPlotAdapter *>("HdbPlotAdapter");
        if(!dbai)
        {
            m_installAdapterInterface(t);
            printf("\e[1;36mhistorical type: creating HDbPlotAdapter\e[0m\n");
        }
        else
            printf("\e[1;36mhistorical type: HDbPlotAdapter \e[1;32myet created\e[0m\n");
    }
    else if(t == THdbBase::Temporary)
    {
        dbai = findChild<TdbPlotAdapter *>("TdbPlotAdapter");
        if(!dbai)
        {
            m_installAdapterInterface(t);
            printf("\e[0;36mTEMPORARY type: creating TDbPlotAdapter\e[0m\n");
        }
        else
            printf("\e[1;36mTEMPORARY type: TDbPlotAdapter \e[1;32myet created\e[0m\n");
    }
    return dbai;
}

void THdbPlot::removeHistoricalCurve(const QString& src)
{
    DbPlotAdapterInterface *pai = getAdapterIf();
    if(pai)
        pai->deleteHistoricalCurve(src);
}


