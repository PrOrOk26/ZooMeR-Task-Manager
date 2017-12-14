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

    lGroup = new QLabel("&Группа:");
    cbGroup = new QComboBox();
    //заполняем список для комбобокса групп
    QSqlQuery query;
    if(!query.exec("SELECT Group_name FROM Group_info")) {
        QMessageBox::critical(0,("obj, cb"),
                        (query.lastError().text()),QMessageBox::Cancel);
    }
    QSqlRecord rec = query.record();
    while (query.next()) {
        cbGroup->addItem(query.value(rec.indexOf("Group_name")).toString());
    }
    lGroup->setBuddy(cbGroup);

    lRegular = new QLabel("&Повторять каждые:");
    sbRegular = new QSpinBox();
    sbRegular->setFixedWidth(50);
    sbRegular->setMinimum(0);
    lRegular->setBuddy(sbRegular);

    lNotification = new QLabel("&Уведомлять за:");
    sbNotification = new QSpinBox();
    sbNotification->setFixedWidth(50);
    sbNotification->setMinimum(0);
    lNotification->setBuddy(sbNotification);

    lDescription = new QLabel("&Описание:");
    teDescription = new QPlainTextEdit();
    teDescription->setFixedHeight(50);
    lDescription->setBuddy(teDescription);

    lWarning = new QLabel("<font color=red>Заполните поле \"Название\"</font");
    lWarning->hide();

    //Layouts
    mlay = new QVBoxLayout();
    layForButton = new QHBoxLayout();
    layForDate = new QHBoxLayout();
    layForReg = new QHBoxLayout();
    layForRem = new QHBoxLayout();
}

void AddObjDialog::SetupLayouts() {
    //add
    layForButton->addStretch();
    layForButton->addWidget(pbSave);
    layForButton->addWidget(pbCancel);

    layForDate->addWidget(deDate);
    layForDate->addWidget(teDate);

    layForReg->addWidget(lRegular);
    layForReg->addWidget(sbRegular);
    layForReg->addWidget(new QLabel("дней"));
    layForRem->addWidget(lNotification);
    layForRem->addWidget(sbNotification);
    layForRem->addWidget(new QLabel("дней"));

    mlay->addWidget(lName);
    mlay->addWidget(leName);

    mlay->addWidget(lDate);
    mlay->addLayout(layForDate);

    mlay->addWidget(lImportance);
    mlay->addWidget(sbImportance);

    mlay->addWidget(lGroup);
    mlay->addWidget(cbGroup);

    mlay->addLayout(layForReg);
    mlay->addLayout(layForRem);

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
