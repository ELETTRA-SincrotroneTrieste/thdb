#include "thdbplotprivate.h"
#include <thdb.h>

THdbPlotPrivate::THdbPlotPrivate(TPlotLightMarker *plot) : q_ptr(plot)
{
    thdb = NULL;
    liveMode = false;
    currentAdapterInterface = NULL;
}
