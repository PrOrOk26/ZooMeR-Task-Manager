#include "mainwin.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Планировщик задач");


    InitializeComponent();
    ReadSettings();
    ConnectSignals();
    SetupLayouts();


}

Widget::~Widget()
{
    WriteSettings();
    delete ui;
}

void Widget::InitializeComponent() {
    settings = new QSettings("ZooMeR", "Планировщик задач", this);
    buttonAddObj = new QPushButton("Добавить задачу");
    buttonAddObj->setDefault(true); // button in focus

    radioAll = new QRadioButton("&Все");
    radioDay = new QRadioButton("&На день");
    radioWeek = new QRadioButton("&На неделю");
    radioMonth = new QRadioButton("&На месяц");

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

void Widget::WriteSettings() {
    settings->beginGroup("/Settings");
        settings->setValue("/width", this->width());
        settings->setValue("/height", this->height());
        settings->setValue("/ra", radioAll->isChecked());
        settings->setValue("/rd", radioDay->isChecked());
        settings->setValue("/rw", radioWeek->isChecked());
        settings->setValue("/rm", radioMonth->isChecked());
    settings->endGroup();
}

void Widget::ShowAllObj() {

}

void Widget::ShowDayObj() {

}

void Widget::ShowWeekObj() {

}

void Widget::ShowMonthObj() {

}



void Widget::slotRadioButtonClicked() {
    if (radioAll->isChecked()) {
        ShowAllObj();
    }
    if (radioDay->isChecked()) {
        ShowDayObj();
    }
    if (radioWeek->isChecked()) {
       ShowWeekObj();
    }
    if (radioMonth->isChecked()) {
       ShowMonthObj();
    }
}


void Widget::slotOpenDialogAddObj() {
    AddObjDialog* dialAdd = new AddObjDialog();
    if (dialAdd->exec() == QDialog::Accepted) {
        //сохранить данные
    }
    delete dialAdd;

}

