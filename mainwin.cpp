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

    radioDay = new QRadioButton("&День");
    radioWeek = new QRadioButton("&Неделю");
    radioMonth = new QRadioButton("&Месяц");
    radioDay->setChecked(true);

    //Layouts
    mainLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    layoutLeftSide = new QVBoxLayout();
    layoutRightSide = new QVBoxLayout();
    layoutForRadio = new QVBoxLayout();
    groupOfRadio = new QGroupBox("&Показать за:");
}

void Widget::SetupLayouts() {
    //add
    layoutLeftSide->addWidget(groupOfRadio);
    layoutLeftSide->addStretch();

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
    connect(radioDay, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioWeek, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));
    connect(radioMonth, SIGNAL(clicked(bool)), SLOT(slotRadioButtonClicked()));

}

void Widget::slotRadioButtonClicked() {
    if (radioDay->isChecked()) {
        //do something
    }
    if (radioWeek->isChecked()) {
        //do something
    }
    if (radioMonth->isChecked()) {
        //do something
    }
}

void Widget::slotOpenDialogAddObj() {
    AddObjDialog* dialAdd = new AddObjDialog();
    if (dialAdd->exec() == QDialog::Accepted) {
        // тут посохранять все данные
    }
    delete dialAdd;

}

