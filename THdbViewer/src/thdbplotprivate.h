#ifndef THDBPLOTPRIVATE_H
#define THDBPLOTPRIVATE_H

class TPlotLightMarker;
class DbPlotAdapterInterface;
class THdb;

#include <QStringList>
#include <thdbbase.h>

class THdbPlotPrivate
{
public:
    THdbPlotPrivate(TPlotLightMarker *plot);

    THdb* thdb;

    bool liveMode;

    QStringList sources;

    DbPlotAdapterInterface *currentAdapterInterface;

    THdbBase::Type currentType;

private:
    Q_DECLARE_PUBLIC(TPlotLightMarker);

    TPlotLightMarker *q_ptr;
};

#endif // THDBPLOTPRIVATE_H
