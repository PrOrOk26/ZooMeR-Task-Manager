#include "addgroupdialog.h"

AddGroupDialog::AddGroupDialog(QWidget *parent) :
    QDialog(parent) {
    this->setWindowTitle("Добавить группу");
    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);

    InitializeComponent();
    SetupLayouts();
    ConnectSignals();
}

void AddGroupDialog::InitializeComponent() {
    lGroupName = new QLabel("Название группы:");
    leGroupName = new QLineEdit();
    pAdd = new QPushButton("Добавить");
    pCancel = new QPushButton("Отмена");
    lWarning = new QLabel("<font color=red>Введите название группы</font>");
    lWarning->hide();
    //Layouts
    mlay = new QVBoxLayout();
    layForButton = new QHBoxLayout();
}

void AddGroupDialog::SetupLayouts() {
    layForButton->addStretch();
    layForButton->addWidget(pAdd);
    layForButton->addWidget(pCancel);

    mlay->addWidget(lGroupName);
    mlay->addWidget(leGroupName);
    mlay->addWidget(lWarning);
    mlay->addLayout(layForButton);

    this->setLayout(mlay);
}

void AddGroupDialog::ConnectSignals() {
    connect(pAdd, SIGNAL(clicked(bool)), SLOT(isFilled()));
    connect(pCancel, SIGNAL(clicked(bool)), SLOT(reject()));
}

void AddGroupDialog::isFilled() {
    if (!(this->leGroupName->text().isEmpty())) this->accept();
    else ShowWarning();
}

void AddGroupDialog::ShowWarning() {
    lWarning->show();
}
