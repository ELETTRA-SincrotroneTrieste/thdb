#include <QtGui/QApplication>
#include "widget.h"
#include <QVariant>

#define CVSVERSION "$Name:  $"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString version(CVSVERSION);
    a.setOrganizationName("Elettra");
    a.setApplicationName("ThdbTestWidget");
    a.setApplicationVersion(version);
    a.setProperty("author", QVariant("Giacomo Strangolino"));
    Widget w;
    w.show();

    return a.exec();
}
