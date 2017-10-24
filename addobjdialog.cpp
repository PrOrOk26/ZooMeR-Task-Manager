#include <addobjdialog.h>

AddObjDialog::AddObjDialog(QWidget *parent) :
    QDialog(parent) {
    this->setWindowTitle("Добавить задачу");
    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);

    InitializeComponent();
    SetupLayouts();
    ConnectSignals();
}

void AddObjDialog::InitializeComponent() {
    pbSave = new QPushButton("Сохранить");
    pbCancel = new QPushButton("Отмена");

    lName = new QLabel("&Название:");
    leName = new QLineEdit();
    lName->setBuddy(leName);

    lDate = new QLabel("&Выполнить до:");
    dteDate = new QDateTimeEdit(QDateTime::currentDateTime());
    lDate->setBuddy(lDate);

    lImportance = new QLabel("&Приоритет:");
    sbImportance = new QSpinBox();
    sbImportance->setRange(1, 5);
    sbImportance->setWrapping(true);
    lImportance->setBuddy(sbImportance);

    lTag = new QLabel("&Тэг:");
    leTag = new QLineEdit();
    lTag->setBuddy(leTag);

    lRegular = new QLabel("&Повторять");
    //тут че-то

    lDescription = new QLabel("&Описание:");
    teDescription = new QPlainTextEdit();
    lDescription->setBuddy(teDescription);

    lNotification = new QLabel("&Уведомлять:");
    cbNotification = new QComboBox();
    QStringList tmp;
    tmp << "Нет" << "В день события" << "За три дня" << "За неделю";
    cbNotification->addItems(tmp);
    lNotification->setBuddy(cbNotification);

    //Layouts
    mlay = new QVBoxLayout();
    layForButton = new QHBoxLayout();
}

void AddObjDialog::SetupLayouts() {
    //add
    layForButton->addStretch();
    layForButton->addWidget(pbSave);
    layForButton->addWidget(pbCancel);

    mlay->addWidget(lName);
    mlay->addWidget(leName);
    mlay->addWidget(lDate);
    mlay->addWidget(dteDate);
    mlay->addWidget(lImportance);
    mlay->addWidget(sbImportance);
    mlay->addWidget(lTag);
    mlay->addWidget(leTag);
    mlay->addWidget(lRegular);
    mlay->addWidget(lDescription);
    mlay->addWidget(teDescription);


    //set
    mlay->addLayout(layForButton);
    setLayout(mlay);
}

void AddObjDialog::ConnectSignals() {
    connect(pbSave, SIGNAL(clicked(bool)), SLOT(accept()));
    connect(pbCancel, SIGNAL(clicked(bool)), SLOT(reject()));
    //connect(cbNotification, SIGNAL(currentIndexChanged(QString)), SLOT()
}
