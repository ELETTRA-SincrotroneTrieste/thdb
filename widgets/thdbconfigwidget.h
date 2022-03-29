#ifndef THDBCONFIGWIDGET_H
#define THDBCONFIGWIDGET_H

#include <QWidget>
#include "ui_configWidget.h"

class EPlotCurve;
class SourcesTreeWidget;

/** \brief a configuration widget for the historical database.
  *
  * This widget contains a set of controls to get data from the historical
  * database.
  */
class THdbConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit THdbConfigWidget(QWidget *parent = 0);

    virtual ~THdbConfigWidget();

    QDateTime startDateTime() const;

    QDateTime stopDateTime() const;

    QStringList sources() const;

    void setSources(const QStringList& src);

    bool live() const;

    bool hdb() const;

    bool tdb() const;

signals:

    void sourceRemoved(const QString& src);

    void liveEnabled(bool);

    void hdbSelected(bool);

    void tdbSelected(bool);

    void viewClicked();

    void cancelClicked();

    void refreshSourcesClicked();

public slots:
    void refreshSources();


protected slots:
    void nowEnabled(bool);

    void addSourceClicked();

    void removeSourceClicked();

//    void addSourcesToggled(bool);

    void updateLiveDateTime();

   // void hdbSelected(bool hdb);

    void lastDaysHoursChanged();

    void init();

private:
    Ui::ConfigWidget ui;

    void m_viewSourcesListChanged();

    QStringList m_sourcesFromTree(QTreeWidget *tree) const;
};

#endif // THDBCONFIGDIALOG_H
