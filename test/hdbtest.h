#ifndef HDBTEST_H
#define HDBTEST_H

#include <QWidget>

class THdb;
class AMTData;
class QTreeWidgetItem;

namespace Ui {
    class HdbTest;
}

class HdbTest : public QWidget
{
    Q_OBJECT

public:
    explicit HdbTest(QWidget *parent = 0);
    ~HdbTest();

private slots:
    void setSource();

    void intervalsSelectionChanged();

    void sourceSelectionChanged();

    void populateSourcesList(const QStringList &);

    void updateAmtData(const QString&, const AMTData&);

    void slotItemClicked(QTreeWidgetItem* , int);

    void clearSelectedCurve();

private:
    Ui::HdbTest *ui;

    THdb *m_thdb;
};

#endif // HDBTEST_H
