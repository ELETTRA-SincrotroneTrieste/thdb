#include "about.h"
#include <QMessageBox>
#include <QCoreApplication>
#include "../../thdbbase.h"

About::About(QObject *parent) :
    QObject(parent)
{
}

void About::about()
{
    QString text;
    text = QString("This panel is a demo using the THdb library.\n"
                   "You can execute any type of query and then click\n"
                   "on the Execute push button.\n"
                   "The results are displayed in an item view on the right\n"
                   "while errors or messages are reported in the text space\n"
                   "at the bottom\nVersion: %1").arg(qApp->applicationVersion());
    QMessageBox::about(0, "About " + qApp->applicationName(), text);
}

void About::aboutQt()
{
    QMessageBox::aboutQt(0, "About Qt library");
}

void About::aboutTHdb()
{
    QMessageBox::about(0, "About THdb library",
                       "THdb database library version:\n" + THdbBase::version());
}
