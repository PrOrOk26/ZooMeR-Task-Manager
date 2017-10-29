#include "mainwin.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // это пошел мой код
    this->setWindowTitle("Планировщик задач");
    InitializeComponent();
    SetupLayouts();
    ConnectSignals();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitializeComponent() {
    buttonAddObj = new QPushButton("Добавить задачу");
    buttonAddObj->setDefault(true); // button in focus

    radioAll = new QRadioButton("&Все");
    radioDay = new QRadioButton("&На день");
    radioWeek = new QRadioButton("&На неделю");
    radioMonth = new QRadioButton("&На месяц");
    radioAll->setChecked(true);

    //Layouts
    mainLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutLeftSide = new QVBoxLayout();
    layoutRightSide = new QVBoxLayout();
    layoutForRadio = new QVBoxLayout();
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

    layoutRightSide->addStretch();
    layoutRightSide->addWidget(buttonAddObj);

    //set
    groupOfRadio->setLayout(layoutForRadio);
    mainLayout->addLayout(layoutLeftSide);
    mainLayout->addLayout(layoutRightSide);

    this->setLayout(mainLayout);
}


void Widget::ConnectSignals() {
    connect(buttonAddObj, SIGNAL(clicked(bool)), SLOT(slotOpenDialogAddObj()));
    connect(radioAll, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioDay, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioWeek, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioMonth, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));

}

void Widget::showAllObj() {
   QTreeWidget* treeAllObj = new QTreeWidget();
   QStringList tmp;
   //tmp << <<
   treeAllObj->setHeaderLabels(tmp);
   treeAllObj->setSortingEnabled(true);


   layoutRightSide->addWidget(treeAllObj);
}

void Widget::showDayObj() {

}

void Widget::showWeekObj() {

}

void Widget::showMonthObj() {

}




void Widget::slotRadioButtonClicked() {
    if (radioAll->isChecked()) {
        showAllObj();
    }
    if (radioDay->isChecked()) {
        showDayObj();
    }
    if (radioWeek->isChecked()) {
       showWeekObj();
    }
    if (radioMonth->isChecked()) {
       showMonthObj();
    }
}

void Widget::slotOpenDialogAddObj() {
    AddObjDialog* dialAdd = new AddObjDialog();
    if (dialAdd->exec() == QDialog::Accepted) {
        // тут посохранять все данные
    }
    delete dialAdd;

}

