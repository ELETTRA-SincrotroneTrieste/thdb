#ifndef THDBSINGLETON_H
#define THDBSINGLETON_H

#include <thdb.h>
#include <QCoreApplication>

/** \brief this class provides a singleton pattern to instantiate a unique
  *        THdb object.
  *
  * \note This singleton instantiates a THdb object and guarantees it to be
  *       unique in your application.
  *
  */
class THdbSingleton : public THdb
{
public:
    static THdbSingleton *instance(QObject *parent = QCoreApplication::instance());

protected:
    THdbSingleton(QObject *parent);

private:
    static THdbSingleton *_instance;
};

#endif // THDBSINGLETON_H
