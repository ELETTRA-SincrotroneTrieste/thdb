#include "thdbsingleton.h"

THdbSingleton *THdbSingleton::_instance = NULL;

THdbSingleton *THdbSingleton::instance(QObject *parent)
 {
     if(!_instance)
         _instance = new THdbSingleton(parent);
     return _instance;
 }

THdbSingleton::THdbSingleton(QObject *parent) : THdb(parent)
{

}
