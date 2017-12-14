#include "mainwin.h"
#include "ui_widget.h"


Widget::~Widget()
{
    delete ui;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Планировщик задач");
    this->setWindowIcon(QIcon(":resource/icon.png"));

    InitializeComponent();
    SetupLayouts();
    ReadSettings();
    ConnectSignals();
    CreateSystemTray();
    BuildModel(actmodel, 0);
    BuildModel(donemodel, 1);
    SetupTree();
    UpdateTree();
    CheckRemember();
}

//Функции конструктора---------------------------------------------
void Widget::InitializeComponent() {
    settings = new QSettings("ZooMeR", "Планировщик задач", this);

    //База данных-----------------------------------
    database = new DB("TaskList");
    treeTask = new QTreeView;
    treeDoneTask = new QTreeView;
    actmodel = new QStandardItemModel;
    donemodel = new QStandardItemModel;

    //-----------------------------------------------
    tabs = new QTabWidget();
    tabs->addTab(treeTask, "Текущие задачи");
    tabs->addTab(treeDoneTask, "Выполненные задачи");

    buttonAddObj = new QPushButton("Добавить задачу");
    buttonAddObj->setDefault(true); // button in focus
    buttonAddGroup = new QPushButton("Добавить группу");

    radioAll = new QRadioButton("&Все");
    radioDay = new QRadioButton("&На день");
    radioWeek = new QRadioButton("&На неделю");
    radioMonth = new QRadioButton("&На месяц");

    //Layouts
    mainLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutLeftSide = new QVBoxLayout();
    layoutRightSide = new QVBoxLayout();
    layoutForRadio = new QVBoxLayout();
    layForButton = new QHBoxLayout();
    groupOfRadio = new QGroupBox("&Показать:");
}

void Widget::SetupLayouts() {

    //add
    layoutLeftSide->addWidget(groupOfRadio);
    layoutLeftSide->addStretch();

    layoutForRadio->addWidget(radioAll);
    layoutForRadio->addWidget(radioDay);
    layoutForRadio->addWidget(radioWeek);
    layoutForRadio->addWidget(radioMonth);

    layForButton->addWidget(buttonAddGroup);
    layForButton->addWidget(buttonAddObj);
    layoutRightSide->addWidget(tabs);
//    layoutRightSide->addStretch();
    layoutRightSide->addLayout(layForButton);

    //set
    groupOfRadio->setLayout(layoutForRadio);
    mainLayout->addLayout(layoutLeftSide, 1);
    mainLayout->addLayout(layoutRightSide, 3);

    this->setLayout(mainLayout);
}

void Widget::ReadSettings() {
    settings->beginGroup("/Settings");
        int sWidth = settings->value("/width", width()).toInt();
        int sHeight = settings->value("/height", height()).toInt();
        bool ra = settings->value("/ra", true).toBool();
        bool rd = settings->value("/rd", false).toBool();
        bool rw = settings->value("/rw", false).toBool();
        bool rm = settings->value("/rm", false).toBool();

        radioAll->setChecked(ra);
        radioDay->setChecked(rd);
        radioWeek->setChecked(rw);
        radioMonth->setChecked(rm);


        this->resize(sWidth, sHeight);
    settings->endGroup();
}

void Widget::ConnectSignals() {
    connect(buttonAddObj, SIGNAL(clicked(bool)), SLOT(slotOpenDialogAddObj()));
    connect(buttonAddGroup, SIGNAL(clicked(bool)), SLOT(slotOpenDialogAddGroup()));
    connect(radioAll, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioDay, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioWeek, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioMonth, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(actmodel, SIGNAL(itemChanged(QStandardItem*)), SLOT(slotItemChanged(QStandardItem*)));
    connect(donemodel, SIGNAL(itemChanged(QStandardItem*)), SLOT(slotItemChanged(QStandardItem*)));
}

void Widget::CreateSystemTray() {
    QAction* pactShowHide = new QAction("&Показать/скрыть окно приложения", this);
    connect(pactShowHide, SIGNAL(triggered()), this, SLOT(slotShowHide()));

    QAction* pactQuit = new QAction("&Выход", this);
    connect(pactQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    mContMenu = new QMenu(this);
    mContMenu->addAction(pactShowHide);
    mContMenu->addAction(pactQuit);

    stTrayIcon = new QSystemTrayIcon(this);
    stTrayIcon->setContextMenu(mContMenu);
    stTrayIcon->setToolTip("Планировщик задач");
    stTrayIcon->setIcon(QPixmap(":resource/icon.png"));

    connect(stTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                               SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));

    stTrayIcon->show();
}

void Widget::EmitCheckSignal() {
    if (radioAll->isChecked()) {
        emit radioAll->clicked(true);
    } else {
        if (radioDay->isChecked()) {
            emit radioDay->clicked(true);
        } else {
            if (radioWeek->isChecked()) {
                emit radioWeek->clicked(true);
            } else {
                if (radioMonth->isChecked()) {
                    emit radioMonth->clicked(true);
                }
            }
        }
    }
}

void Widget::CheckRemember() {
    QSqlQuery query;
    query.exec("SELECT * "
               "FROM Task_info, Task_date_time "
               "WHERE Task_info.Task_id = Task_date_time.Task_id ");
    QSqlRecord rec = query.record();
    while(query.next()) {
        qint64 rem = query.value(rec.indexOf("Remember")).toInt();
         if (rem != 0) {
             if (rem == QDate::currentDate().daysTo(query.value(rec.indexOf("Date")).toDate())) {
               ShowRemember(query.value(rec.indexOf("Task_name")).toString(), rem);
               return;
             }
        }

    }
}
//----------------------------------------------------------------

//Управление треем------------------------------------------------
void Widget::slotShowHide() {
    setVisible(!isVisible());
}

void Widget::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        setVisible(true);
    }
}
//----------------------------------------------------------------

//Работа с группами------------------------------------------------
void Widget::slotOpenDialogAddGroup() {
    AddGroupDialog* dialGroup = new AddGroupDialog();
    connect(dialGroup, SIGNAL(StartInsert(QString)),  SLOT(slotStartInsert(QString)));
    connect(this, SIGNAL(ShowWarning(TypeOfWarning)), dialGroup, SLOT(slotShowWarning(TypeOfWarning)));
    connect(this, SIGNAL(AcceptGroup()), dialGroup, SLOT(accept()));
    dialGroup->exec();

    delete dialGroup;
}

void Widget::slotStartInsert(QString group_name) {
    QList<QVariant> grouplst;
    grouplst.push_back(group_name);
    if(!database->InsertData(TypeOfInsert::GROUP, grouplst)) {
        emit ShowWarning(TypeOfWarning::NOT_UN);
    }
    else {
        emit AcceptGroup();
        EmitCheckSignal();
    }
}
//----------------------------------------------------------------

//Работа с задачами-----------------------------------------------
void Widget::slotOpenDialogAddObj() {
    AddObjDialog* dialAdd = new AddObjDialog();
    if (dialAdd->exec() == QDialog::Accepted) {
        QList<QVariant> list;
        list.push_back(dialAdd->sbRegular->text());
        list.push_back(dialAdd->leName->text());
        if (dialAdd->teDescription->toPlainText().isEmpty()) {
            list.push_back("---");
        }
        else {list.push_back(dialAdd->teDescription->toPlainText());}
        list.push_back(dialAdd->cbGroup->currentIndex());
        list.push_back(dialAdd->sbImportance->text());
        list.push_back(dialAdd->sbNotification->text());
        list.push_back(dialAdd->deDate->date());
        list.push_back(dialAdd->teDate->time());
        database->InsertData(TypeOfInsert::TASK, list);
    }
    EmitCheckSignal();
    delete dialAdd;
}

void Widget::slotItemChanged(QStandardItem* item) {
    QSqlQuery query;
    QStandardItem* parent = item->parent();
    int row = item->row();
    int col = item->column();
    if(col == 0) {
        if (parent == 0) {
            QString group = item->text();
            QString id = item->model()->takeItem(row, item->model()->columnCount()-1)->text();
            query.exec("UPDATE Group_info "
                       "SET Group_name = '" + group +
                       "' WHERE Group_id = " + id);
        } else {
            if(item->isCheckable()) {
                if(item->checkState() == Qt::Checked) {
                    QStandardItem* itid = parent->takeChild(row, parent->columnCount()-1);
                    QString id = itid->text();
                    if(!query.exec("SELECT Done "
                               "FROM Task_done_rep "
                               "WHERE Task_id = " + id)) {
                        QMessageBox::critical(0,("2"),
                                    (query.lastError().text()),QMessageBox::Cancel);
                    }
                    query.next();
                    bool done = !(query.value(query.record().indexOf("Done")).toBool());
                    if(!query.exec("UPDATE Task_done_rep "
                               "SET Done = " + QVariant(QVariant(done).toInt()).toString() +
                               " WHERE Task_id = " + id)) {
                        QMessageBox::critical(0,("4"),
                                    (query.lastError().text()),QMessageBox::Cancel);
                    }
                    Repeat(id);
                } else {
                    QStandardItem* itid = parent->takeChild(row, parent->columnCount()-1);
                    QString id = itid->text();
                    query.exec("UPDATE Task_info "
                               "SET Task_name = '" + item->text() +
                               "' WHERE Task_id = " +id);
                }
            }
        }
    }
    if (col == item->model()->columnCount()-2) {
        QString id = parent->takeChild(row, parent->columnCount()-1)->text();
        query.exec("UPDATE Task_info "
                   "SET Importance = " + item->text() +
                   " WHERE Task_id = " +id);
    }
    EmitCheckSignal();
}
//----------------------------------------------------------------

//Вспомогательные функции-----------------------------------------
void Widget::ShowRemember(QString remember, qint64 n) {
    stTrayIcon->showMessage("Выполните задачу", remember + "\n Крайний срок: " +
                            QDate::currentDate().addDays(n).toString(),
                            QSystemTrayIcon::Information, 7000);
}
//----------------------------------------------------------------

//Закрытие окна---------------------------------------------------
void Widget::closeEvent(QCloseEvent* pe) {
    WriteSettings();
    hide();
}

void Widget::WriteSettings() {
    settings->beginGroup("/Settings");
        settings->setValue("/width", this->width());
        settings->setValue("/height", this->height());
        settings->setValue("/ra", radioAll->isChecked());
        settings->setValue("/rd", radioDay->isChecked());
        settings->setValue("/rw", radioWeek->isChecked());
        settings->setValue("/rm", radioMonth->isChecked());
        for (int i = 0; i < actmodel->columnCount(); ++i) {
            settings->setValue("/ac"+ QVariant(i).toString(), treeTask->columnWidth(i));
        }
        for (int i = 0; i < donemodel->columnCount(); ++i) {
            settings->setValue("/dc"+ QVariant(i).toString(), treeDoneTask->columnWidth(i));
        }

    settings->endGroup();
}
//----------------------------------------------------------------


//Построение модели-----------------------------------------------
void Widget::SetupTree() {
    treeTask->setModel(actmodel);
    treeDoneTask->setModel(donemodel);

    treeTask->setSortingEnabled(true);
    treeDoneTask->setSortingEnabled(true);
    treeTask->header()->setCascadingSectionResizes(true);
    treeDoneTask->header()->setCascadingSectionResizes(true);

    treeTask->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    treeDoneTask->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    treeTask->setDragEnabled(true);
//    treeTask->setAcceptDrops(true);
//    treeTask->setDropIndicatorShown(true);
//    treeTask->setDragDropMode(QAbstractItemView::InternalMove);
//    treeDoneTask->setDragEnabled(true);
//    treeDoneTask->setAcceptDrops(true);
//    treeDoneTask->setDropIndicatorShown(true);
//    treeDoneTask->setDragDropMode(QAbstractItemView::InternalMove);
}

void Widget::UpdateTree() {
    treeTask->hideColumn(actmodel->columnCount()-1);
    treeDoneTask->hideColumn(donemodel->columnCount()-1);

    for (int i = 0; i < actmodel->columnCount(); ++i) {
           treeTask->setColumnWidth(i, settings->value("/Settings/ac" + QVariant(i).toString(), 50).toInt());
    }
    for (int i = 0; i < donemodel->columnCount(); ++i) {
            treeDoneTask->setColumnWidth(i, settings->value("/Settings/dc" + QVariant(i).toString(), 50).toInt());
    }

//    treeTask->expandAll();
//    treeDoneTask->expandAll();
}

void Widget::BuildModel(QStandardItemModel *model, int donenumb) {
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("Имя"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Выполнить до"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Приоритет"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Id"));

    QString sQuery;
    QStandardItem* top = model->invisibleRootItem();
    QSqlQuery query1, query2;
    query1.exec("SELECT Group_id, Group_name FROM Group_info");
    QSqlRecord reclvl1 = query1.record();
    QList<QStandardItem*> lvl1;
    while(query1.next()) {
        QString n = query1.value(reclvl1.indexOf("Group_id")).toString();
        lvl1.push_back(new QStandardItem(query1.value(reclvl1.indexOf("Group_name")).toString()));
        lvl1.push_back(new QStandardItem());
        lvl1.push_back(new QStandardItem());
        lvl1.push_back(new QStandardItem(n));
        top->appendRow(lvl1);


    if (radioAll->isChecked()) {
        sQuery = "SELECT Task_info.Task_id, Task_info.Task_name, "
                 "Task_info.Task_descr, Task_info.Task_gr_id, "
                 "Task_info.Importance, "
                 "Task_date_time.Date, Task_date_time.Time "
                  "FROM Task_info, Task_date_time "
                  "WHERE "
                 "Task_info.Task_id = Task_date_time.Task_id "
                 "AND "
                 "Task_info.Task_id IN "
                 "(SELECT Task_done_rep.Task_id "
                 "FROM Task_done_rep "
                 "WHERE Task_done_rep.Done = " + QVariant(donenumb).toString()+
                 ")"
                 "AND "
                  "Task_info.Task_gr_id = " + n;
    }
    if (radioDay->isChecked()) {
        sQuery = "SELECT Task_info.Task_id, Task_info.Task_name, "
                 "Task_info.Task_descr, Task_info.Task_gr_id, "
                 "Task_info.Importance, "
                 "Task_date_time.Date, Task_date_time.Time "
                  "FROM Task_info, Task_date_time "
                  "WHERE "
                  "Task_info.Task_id = Task_date_time.Task_id "
                  "AND "
                     "Task_info.Task_id IN "
                     "(SELECT Task_done_rep.Task_id "
                     "FROM Task_done_rep "
                     "WHERE Task_done_rep.Done =  "
                     + QVariant(donenumb).toString()+
                    ")"
                    "AND "
                     "Task_info.Task_id IN "
                     "( "
                     "SELECT Task_id "
                     "FROM Task_date_time "
                     "WHERE Task_date_time.Date = '"
                    + QVariant(QDateEdit(QDate::currentDate()).date()).toString() + "'"
                     ") "
                     "AND "
                      "Task_info.Task_gr_id = " + n;
    }
    if (radioWeek->isChecked()) {
        sQuery = "SELECT Task_info.Task_id, Task_info.Task_name, "
                 "Task_info.Task_descr, Task_info.Task_gr_id, "
                 "Task_info.Importance, "
                 "Task_date_time.Date, Task_date_time.Time "
                  "FROM Task_info, Task_date_time "
                  "WHERE "
                  "Task_info.Task_id = Task_date_time.Task_id "
                  "AND "
                     "Task_info.Task_id IN "
                     "(SELECT Task_done_rep.Task_id "
                     "FROM Task_done_rep "
                     "WHERE Task_done_rep.Done = " + QVariant(donenumb).toString()+
                     ")"
                     "AND "
                     "Task_info.Task_id IN "
                     "( "
                     "SELECT Task_id "
                     "FROM Task_date_time "
                     "WHERE Task_date_time.Date BETWEEN '"
                    + QVariant(QDateEdit(QDate::currentDate()).date()).toString() +
                    "'"
                    "AND '" +
                    QVariant(QDateEdit(QDate::currentDate().addDays(7)).date()).toString() +
                     "') "
                     "AND "
                      "Task_info.Task_gr_id = " + n;
    }
    if (radioMonth->isChecked()) {
        sQuery = "SELECT Task_info.Task_id, Task_info.Task_name, "
                 "Task_info.Task_descr, Task_info.Task_gr_id, "
                 "Task_info.Importance, "
                 "Task_date_time.Date, Task_date_time.Time "
                  "FROM Task_info, Task_date_time "
                  "WHERE "
                  "Task_info.Task_id = Task_date_time.Task_id "
                  "AND "
                     "Task_info.Task_id IN "
                     "(SELECT Task_done_rep.Task_id "
                     "FROM Task_done_rep "
                     "WHERE Task_done_rep.Done = " + QVariant(donenumb).toString()+
                     ")"
                     "AND "
                     "Task_info.Task_id IN "
                     "( "
                     "SELECT Task_id "
                     "FROM Task_date_time "
                     "WHERE Task_date_time.Date BETWEEN '"
                    + QVariant(QDateEdit(QDate::currentDate()).date()).toString() +
                    "'"
                    "AND '" +
                    QVariant(QDateEdit(QDate::currentDate().addDays(30)).date()).toString() +
                     "') "
                     "AND "
                      "Task_info.Task_gr_id = " + n;
    }

        if(!query2.exec(sQuery))
        {
            QMessageBox::critical(0,("Error"),
                        (query2.lastError().text()),QMessageBox::Cancel);
        }
        QSqlRecord reclvl2 = query2.record();
        QList<QStandardItem*> lvl2;
        while(query2.next()) {
            lvl2.push_back(new QStandardItem(query2.value(reclvl2.indexOf("Task_name")).toString()));
            lvl2.push_back(new QStandardItem(QString(query2.value(reclvl2.indexOf("Date")).toString() + " "
                                             + query2.value(reclvl2.indexOf("Time")).toString())));
            lvl2.push_back(new QStandardItem(query2.value(reclvl2.indexOf("Importance")).toString()));
            lvl2.push_back(new QStandardItem(query2.value(reclvl2.indexOf("Task_id")).toString()));
            lvl2[0]->setCheckable(true);
            lvl1[0]->appendRow(lvl2);
            lvl2[0]->appendRow(new QStandardItem(query2.value(reclvl2.indexOf("Task_descr")).toString()));

            lvl2.clear();
        }
        lvl1.clear();
    }
}

void Widget::slotRadioButtonClicked() {
    BuildModel(actmodel, 0);
    BuildModel(donemodel, 1);
    UpdateTree();
}
//----------------------------------------------------------------


void Widget::Repeat(QString id) {
    QSqlQuery query;
    query.exec("SELECT * "
               "FROM Task_done_rep "
               "WHERE Task_id = " + id);
    query.next();
    bool done = query.value(query.record().indexOf("Done")).toBool();
    int rep = query.value(query.record().indexOf("Repeat")).toInt();
    if (rep != 0 && done) {
        query.exec("UPDATE Task_done_rep "
                   "SET Done = 0,"
                   "Repeat = " + QVariant(rep).toString()+
                   " WHERE Task_id = " + id);
        query.exec("SELECT Date "
                   "FROM Task_date_time "
                   "WHERE Task_id = " +id);
        query.next();
        QDateEdit de(query.value(query.record().indexOf("Date")).toDate().addDays(rep));
        query.exec("UPDATE Task_date_time "
                   "SET Date = '" + QVariant(de.date()).toString() +
                   "' WHERE Task_id = " + id);
    }
    //EmitCheckSignal();
}
