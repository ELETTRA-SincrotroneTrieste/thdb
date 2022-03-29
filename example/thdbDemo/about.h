#ifndef ABOOUT_H
#define ABOOUT_H

#include <QObject>

class About : public QObject
{
    Q_OBJECT
public:
    explicit About(QObject *parent = 0);

signals:

public slots:
    void about();
    void aboutTHdb();
    void aboutQt();

};

#endif // ABOOUT_H
