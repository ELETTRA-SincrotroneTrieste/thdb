#ifndef THDBPLOT_H
#define THDBPLOT_H

#include <TPlotLightMarker>
#include <QDateTime>
#include "../thdb.h"
#include "../dbconnectionparams.h"

class THdbPlotPrivate;
class DbPlotAdapterInterface;

class THdbPlot : public TPlotLightMarker
{
    Q_OBJECT
public:
    THdbPlot(QWidget *parent);

    virtual ~THdbPlot();

    void setTHdb(THdb* t);

    THdb* thdb() const;

    void setType(THdbBase::Type t);

    THdbBase::Type type() const;

    bool liveModeEnabled() const;

signals:

    /** \brief Emitted when new data arrives from hdb.
      */
    void historicalDataUpdated();

public slots:

    void update(const QStringList& sources);

    void removeHistoricalCurve(const QString& src);

    void setLiveModeEnabled(bool live);


    /** \brief if parent widget is a HistoricalViewWidget, then asks it to
      *        switch to the correct widget aimed at represented Number Scalars
      *
      */
    void requestWidgetSwitch();

    DbPlotAdapterInterface *getAdapterIf();

protected slots:
    virtual void databaseChanged(THdbBase::Type);

private:

    void m_installAdapterInterface(THdbBase::Type t);

    Q_DECLARE_PRIVATE(THdbPlot)
    THdbPlotPrivate *d_ptr;

};

#endif // THDBPLOT_H
