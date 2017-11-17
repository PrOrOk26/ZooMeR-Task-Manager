#include <addobjdialog.h>

AddObjDialog::AddObjDialog(QWidget *parent) :
    QDialog(parent) {
    this->setWindowTitle("Добавить задачу");
    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);

    InitializeComponent();
    ReadSettings();
    SetupLayouts();
    ConnectSignals();
}
AddObjDialog::~AddObjDialog() {
    WriteSettings();
}

void AddObjDialog::InitializeComponent() {
    settings = new QSettings("ZooMeR", "Планировщик задач", this);

    pbSave = new QPushButton("Сохранить");
    pbCancel = new QPushButton("Отмена");

    lName = new QLabel("&Название:");
    leName = new QLineEdit();
    lName->setBuddy(leName);

    lDate = new QLabel("&Выполнить до:");
    deDate = new QDateEdit(QDate::currentDate().addDays(1));
    teDate = new QTimeEdit(QTime(23, 0));
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
    teDescription->setFixedHeight(50);
    lDescription->setBuddy(teDescription);

    lNotification = new QLabel("&Уведомлять:");
    cbNotification = new QComboBox();
    QStringList tmp;
    tmp << "Нет" << "В день события" << "За три дня" << "За неделю";
    cbNotification->addItems(tmp);
    lNotification->setBuddy(cbNotification);

    lWarning = new QLabel("<font color=red>Заполните поле \"Название\"</font");
    lWarning->hide();

    //Layouts
    mlay = new QVBoxLayout();
    layForButton = new QHBoxLayout();
    layForDate = new QHBoxLayout();
}

void AddObjDialog::SetupLayouts() {
    //add
    layForButton->addStretch();
    layForButton->addWidget(pbSave);
    layForButton->addWidget(pbCancel);

    layForDate->addWidget(deDate);
    layForDate->addWidget(teDate);

    mlay->addWidget(lName);
    mlay->addWidget(leName);

    mlay->addWidget(lDate);
    mlay->addLayout(layForDate);

    mlay->addWidget(lImportance);
    mlay->addWidget(sbImportance);

    mlay->addWidget(lTag);
    mlay->addWidget(leTag);

    mlay->addWidget(lRegular);

    mlay->addWidget(lDescription);
    mlay->addWidget(teDescription);

    mlay->addWidget(lWarning);

    mlay->addLayout(layForButton);

    //set
    setLayout(mlay);
}

void AddObjDialog::ConnectSignals() {
    connect(pbSave, SIGNAL(clicked(bool)), SLOT(isFilled()));
    connect(pbCancel, SIGNAL(clicked(bool)), SLOT(reject()));
}

void AddObjDialog::isFilled() {
    if (!(this->leName->text().isEmpty())) this->accept();
    else ShowWarning();
}

void AddObjDialog::ShowWarning() {
    lWarning->show();
}

void AddObjDialog::ReadSettings() {
    settings->beginGroup("/Settings");
        int sW = settings->value("/w", width()).toInt();
        int sH = settings->value("/h", height()).toInt();

        this->resize(sW, sH);
    settings->endGroup();
}

void AddObjDialog::WriteSettings() {
    settings->beginGroup("/Settings");
        settings->setValue("/w", this->width());
        settings->setValue("/h", this->height());
    settings->endGroup();
}
