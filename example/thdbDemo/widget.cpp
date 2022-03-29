#include "widget.h"
#include "about.h" /* manages Help->about... menu in menubar */
#include "ui_widget.h"
#include <QHeaderView>

/* to save query history */
#include <QSettings>

/* the following includes are necessary inside notify */
#include <QSqlRecord>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <stdio.h>

/* these are needed to open a database connection and to register the
 * desired query.
 */
#include <THdbBase> /* to connect to db and register MyQuery */
#include <DbConnectionParams> /* to setup connection paramters */

void MyQuery::notify(const QSqlQuery &constQuery, const int eventId)
{
    QStringList headerItems;
    /* in this case eventId is ignored, but in general it can be
     * used to recognize the query the data comes from.
     */
    Q_UNUSED(eventId);
    QSqlQuery q(constQuery);

    if(q.lastError().type() != QSqlError::NoError) /* error handling */
    {
        emit message(q.lastError().text());
    }
    else
    {
        /* get number of columns and column names */
        QSqlRecord record = q.record();


        for(int i = 0; i < record.count(); i++)
            headerItems << record.fieldName(i);
        emit header(headerItems);

        /* extract rows and columns */
        while(q.next())
        {
            QStringList rowItems;
            for(int i = 0; i < record.count(); i++)
                rowItems << q.value(i).toString();
            /* notify a new row has been extracted */
            emit rowReady(rowItems);
        }

        /* query ok message */
        if(record.count())
            emit message(QString("Query ok %1 columns, %2 rows").
                         arg(record.count()).arg(q.size()));
        else if(q.isActive()) /* was successfully executed */
            emit message("Query successfully executed");

        emit querySuccessful(q.lastQuery(), q.isActive());
    }
}

Widget::Widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Widget)
{
    QSettings s;
    ui->setupUi(this);
    ui->treeWidget->setAlternatingRowColors(true);
    ui->treeWidget->header()->setResizeMode(QHeaderView::Interactive);
    connect(ui->pbConnect, SIGNAL(clicked()), this, SLOT(openDbConnection()));
    THdbBase *db = new THdbBase(this);
    connect(db, SIGNAL(dbConnectionInfo(QString)), ui->leErrors, SLOT(setText(QString)));
    connect(db, SIGNAL(queryExecuted(bool)), ui->gbResult, SLOT(setChecked(bool)));
    /* the following signal is emitted when the database thread starts executing
     * the query, in another thread, the Database thread.
     */
    connect(db, SIGNAL(querySent()), this, SLOT(querySent()));

    /* instantiate and register our MyQuery object */
    MyQuery *myQuery = new MyQuery(this);
    /* the string as first argument is a mnemonic one. It can be any string,
     * it can be lately used to get myQuery through queryInterface() method.
     */
    db->registerDbQuery("MyQuery", myQuery);

    connect(ui->pbExecute, SIGNAL(clicked()), this, SLOT(executeQuery()));

    /* history combobox currentChanged fills in Query text edit */
    connect(ui->comboHistory, SIGNAL(currentIndexChanged(QString)), ui->teQuery,
            SLOT(setText(QString)));
    /* fill in combo box for history */
    ui->comboHistory->insertItems(0, s.value("QUERY_HISTORY_"
                     + getHistorySettingsKey(), "SHOW TABLES").toStringList());
     /* select last */
    ui->comboHistory->setCurrentIndex(ui->comboHistory->count() - 1);

    /* load connection parameters that were used last time */
    loadLastConnectionParams();

    /* when query results are ready, populate table widget */
    connect(myQuery, SIGNAL(header(QStringList)), this, SLOT(setTreeHeader(QStringList)));
    connect(myQuery, SIGNAL(rowReady(QStringList)), this, SLOT(addTreeItem(QStringList)));
    connect(myQuery, SIGNAL(message(QString)), ui->leErrors, SLOT(setText(QString)));
    connect(myQuery, SIGNAL(querySuccessful(QString,bool)), this, SLOT(querySuccessful(QString,bool)));

    /* About actions in Help menubar. It's not relevant as far as THdb is concerned */
    About *about = new About(this);
    connect(ui->actionAbout, SIGNAL(triggered()), about, SLOT(about()));
    connect(ui->actionAbout_THdb, SIGNAL(triggered()), about, SLOT(aboutTHdb()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), about, SLOT(aboutQt()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

Widget::~Widget()
{
    storeLastConnectionParams();
    delete ui;
}

void Widget::openDbConnection()
{
    DbConnectionParams params(ui->leSchema->text(),
                              ui->leUser->text(),
                              ui->lePassword->text(),
                              ui->leHost->text(),
                              ui->sbPort->value());

    THdbBase *db = findChild<THdbBase *>();
    db->setType(THdbBase::User, params);
}

void Widget::setTreeHeader(const QStringList& items)
{
    ui->treeWidget->setHeaderLabels(items);
}

void Widget::addTreeItem(const QStringList &row)
{
    new QTreeWidgetItem(ui->treeWidget, row);
}

void Widget::executeQuery()
{
    /* clear table */
    ui->treeWidget->clear(); /* remove items */
    ui->treeWidget->setColumnCount(0); /* clear header items */

    QString query = ui->teQuery->toPlainText();
    query.remove("\n");

    THdbBase *db = findChild<THdbBase *>();

    /* obtain the registered query. We must static_cast it to treat it
     * as a MyQuery object and to be able to call its specific methods,
     * such as setQuery.
     */
    MyQuery *myQuery = static_cast<MyQuery *>(db->dbQueryInterface("MyQuery"));
    myQuery->setQuery(query);
    myQuery->execute();
}

void Widget::querySuccessful(const QString& query, bool success)
{
    QSettings s;
    QStringList history = s.value("QUERY_HISTORY_"  + getHistorySettingsKey()).toStringList();
    QApplication::restoreOverrideCursor();

    while(history.size() >= 20)
    {
        ui->comboHistory->removeItem(ui->comboHistory->findText(history.first()));
        history.removeFirst();
    }
    if(success && ui->comboHistory->findText(query) == -1)
    {
        history << query;
        s.setValue("QUERY_HISTORY_" + getHistorySettingsKey(), history);
        ui->comboHistory->insertItem(ui->comboHistory->count(), query);
        ui->comboHistory->setCurrentIndex(ui->comboHistory->findText(query));
    }
    /* resize tree header section to adapt to contents */
    ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
}

void Widget::querySent()
{
    ui->leErrors->setText("Query sent to database");
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void Widget::loadLastConnectionParams()
{
    QSettings s;
    ui->leHost->setText(s.value("DBHOST_" + getHistorySettingsKey()).toString());
    ui->leUser->setText(s.value("DBUSER_" + getHistorySettingsKey()).toString());
    ui->lePassword->setText(s.value("DBPASSWORD_" + getHistorySettingsKey()).toString());
    ui->leSchema->setText(s.value("DBSCHEMA_" + getHistorySettingsKey()).toString());
    ui->sbPort->setValue(s.value("DBPORT_" + getHistorySettingsKey(), 3306).toInt());
}

void Widget::storeLastConnectionParams()
{
    QSettings s;
    s.setValue("DBHOST_" + getHistorySettingsKey(), ui->leHost->text());
    s.setValue("DBUSER_" + getHistorySettingsKey(), ui->leUser->text());
    s.setValue("DBPASSWORD_" + getHistorySettingsKey(), ui->lePassword->text());
    s.setValue("DBSCHEMA_" + getHistorySettingsKey(), ui->leSchema->text());
    s.setValue("DBPORT_" + getHistorySettingsKey(), ui->sbPort->value());
}

QString Widget::getHistorySettingsKey()
{
    QString user;
    char *p = getenv("USER");
    if(p)
        user = QString(p);
    return user;
}

