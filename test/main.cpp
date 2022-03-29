#include <QtGui/QApplication>
#include "hdbtest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HdbTest w;
    w.show();

    return a.exec();
}
