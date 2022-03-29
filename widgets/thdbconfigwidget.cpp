#include "thdbconfigwidget.h"
#include "archivedsourcestreewidget.h"
#include "dbconnectionparams.h"
#include <macros.h>
#include <thdbsingleton.h>
#include <QTimer>
#include <QtDebug>

THdbConfigWidget::~THdbConfigWidget()
{
    qDebug() << __FUNCTION__;
}

THdbConfigWidget::THdbConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    resize(minimumSizeHint());

    QTimer::singleShot(400, this, SLOT(init()));

    m_viewSourcesListChanged();
}

void THdbConfigWidget::init()
{
    connect(ui.cbLastDaysHours, SIGNAL(toggled(bool)), ui.sbDaysHours, SLOT(setEnabled(bool)));
    connect(ui.cbLastDaysHours, SIGNAL(toggled(bool)), ui.cbDaysHours, SLOT(setEnabled(bool)));
    connect(ui.cbNow, SIGNAL(toggled(bool)), ui.teStop, SLOT(setDisabled(bool)));
    connect(ui.cbLive, SIGNAL(toggled(bool)), ui.cbNow, SLOT(setChecked(bool)));
    connect(ui.cbLive, SIGNAL(toggled(bool)), this, SIGNAL(liveEnabled(bool)));
    connect(ui.cbNow, SIGNAL(toggled(bool)),this, SLOT(nowEnabled(bool)));
    connect(ui.pbAddSrc, SIGNAL(clicked()), this, SLOT(addSourceClicked()));
    connect(ui.rbHdb, SIGNAL(toggled(bool)), this, SIGNAL(hdbSelected(bool)));
    connect(ui.rbTdb, SIGNAL(toggled(bool)), this, SIGNAL(tdbSelected(bool)));
//    connect(ui.pbAddSources, SIGNAL(toggled(bool)), ui.gbEditSources, SLOT(setVisible(bool)));
//    connect(ui.pbAddSources, SIGNAL(toggled(bool)), ui.gbEditSources, SLOT(setChecked(bool)));
//    connect(ui.gbEditSources, SIGNAL(clicked(bool)), ui.pbAddSources, SLOT(setChecked(bool)));
//    connect(ui.pbAddSources, SIGNAL(toggled(bool)), this, SLOT(addSourcesToggled(bool)));

    connect(ui.pbRemoveSrc, SIGNAL(clicked()), this, SLOT(removeSourceClicked()));
    connect(ui.pbRefreshSources, SIGNAL(clicked()), this, SLOT(refreshSources()));

    connect(ui.pbView, SIGNAL(clicked()), this, SIGNAL(viewClicked()));
    connect(ui.pbCancel, SIGNAL(clicked()), this, SIGNAL(cancelClicked()));

    connect(ui.rbHdb, SIGNAL(toggled(bool)), this, SLOT(refreshSources()));

    QTimer *liveTimer = new QTimer(this);
    liveTimer->setObjectName("LiveTimer");
    liveTimer->setSingleShot(false);
    connect(liveTimer, SIGNAL(timeout()), this, SLOT(updateLiveDateTime()));

    ui.cbLastDaysHours->setChecked(false);

    QDateTime dt = QDateTime::currentDateTime();
    ui.calStop->setSelectedDate(dt.date());
    ui.teStop->setTime(dt.time());
    dt = dt.addDays(-1);
    ui.calStart->setSelectedDate(dt.date());
    ui.teStart->setTime(dt.time());

    ui.sbDaysHours->setDisabled(true);
    ui.cbDaysHours->setDisabled(true);
    connect(ui.sbDaysHours, SIGNAL(valueChanged(int)), this, SLOT(lastDaysHoursChanged()));
    connect(ui.cbDaysHours, SIGNAL(currentIndexChanged(int)), this, SLOT(lastDaysHoursChanged()));

    /* load archived attribute list from the database */
    refreshSources();
}

void THdbConfigWidget::setSources(const QStringList &src)
{
    foreach(QString source, src)
        ui.twSelected->addSource(source, true);
    m_viewSourcesListChanged();
}

QDateTime THdbConfigWidget::startDateTime() const
{
    QDateTime dt;
    dt.setDate(ui.calStart->selectedDate());
    dt.setTime(ui.teStart->time());
    return dt;
}

QDateTime THdbConfigWidget::stopDateTime() const
{
    QDateTime dt;
    dt.setDate(ui.calStop->selectedDate());
    dt.setTime(ui.teStop->time());
    return dt;
}

void THdbConfigWidget::nowEnabled(bool en)
{
    QTimer *t = findChild<QTimer *>("LiveTimer");
    en ? t->start() : t->stop();
}

QStringList THdbConfigWidget::m_sourcesFromTree(QTreeWidget *tree) const
{
    QStringList ret;
    SourcesTreeWidget *stw = findChild<SourcesTreeWidget *>(tree->objectName());
    if(stw)
        ret = stw->getSources();
    qDebug() << "THdbConfigWidget::m_sourcesFromTree() " << ret;
    return ret;
}

QStringList THdbConfigWidget::sources() const
{
    qDebug() << "sources() " << m_sourcesFromTree(ui.twSelected);
    return m_sourcesFromTree(ui.twSelected);
}

bool THdbConfigWidget::live() const
{
    return ui.cbLive->isChecked();
}

void THdbConfigWidget::addSourceClicked()
{
    QStringList srcs = ui.twArchived->selectedSources();
    foreach(QString src, srcs)
        ui.twSelected->addSource(src, true);
    m_viewSourcesListChanged();
}

void THdbConfigWidget::removeSourceClicked()
{
    QStringList srcs = ui.twSelected->selectedSources();
    foreach(QString src, srcs)
        emit sourceRemoved(src);
    ui.twSelected->removeSelectedSources();
    m_viewSourcesListChanged();
}

void THdbConfigWidget::m_viewSourcesListChanged()
{
    ui.gbFrom->setEnabled(sources().size() > 0);
    ui.gbTo->setEnabled(sources().size() > 0);

    QStringList srcs = sources();
    int ssize = srcs.size();

    if(ssize > 1 )
        ui.labelSource->setText(QString("%1, %2...").arg(srcs.first(), srcs.at(1)));
    else if(ssize == 1)
        ui.labelSource->setText(srcs.first());
}

void THdbConfigWidget::refreshSources()
{
    if(ui.rbHdb->isChecked())
        ui.twArchived->switchDatabase(THdbBase::Historical, HdbConnectionParams());
    else
        ui.twArchived->switchDatabase(THdbBase::Temporary, TdbConnectionParams());
}

void THdbConfigWidget::updateLiveDateTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    ui.calStop->setSelectedDate((dt.date()));
    ui.teStop->setTime(dt.time());
}

bool THdbConfigWidget::hdb() const
{
    return ui.rbHdb->isChecked();
}

bool THdbConfigWidget::tdb() const
{
    return ui.rbTdb->isChecked();
}

void THdbConfigWidget::lastDaysHoursChanged()
{
    QDateTime dt = QDateTime::currentDateTime();
    ui.calStop->setSelectedDate(dt.date());
    ui.teStop->setTime(dt.time());
    switch(ui.cbDaysHours->currentIndex())
    {
    case 0: /* days */
        dt = dt.addDays(-ui.sbDaysHours->value());
        break;
       case 1: /* Hours */
        dt.addSecs(-ui.sbDaysHours->value() * 3600);
        break;
    case 2:
        dt.addSecs(-ui.sbDaysHours->value() * 60);
        break;
    case 3:
        dt.addSecs(-ui.sbDaysHours->value());
        break;
    default:
        break;
    }
    ui.calStart->setSelectedDate(dt.date());
    ui.teStart->setTime(dt.time());
}


