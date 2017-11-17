#ifndef ADDOBJDIALOG_H
#define ADDOBJDIALOG_H

#include <QWidget>
#include <QtWidgets>
#include <QSettings>

class AddObjDialog : public QDialog {
    Q_OBJECT
private:
    QPushButton* pbSave;
    QPushButton* pbCancel;

    QLabel* lName;
    QLabel* lDate;
    QLabel* lImportance;
    QLabel* lTag;
    QLabel* lRegular;
    QLabel* lDescription;
    QLabel* lNotification;

    QLabel* lWarning;

    QLineEdit* leName;
    QDateEdit* deDate;
    QTimeEdit* teDate;
    QSpinBox* sbImportance;
    QLineEdit* leTag;

    QSettings* settings;
    //повтор
    QPlainTextEdit* teDescription;
    QComboBox* cbNotification;

    //Layouts
    QVBoxLayout* mlay;
    QHBoxLayout* layForButton;
    QHBoxLayout* layForDate;

    //Functions
    void InitializeComponent();
    void SetupLayouts();
    void ConnectSignals();

    void WriteSettings();
    void ReadSettings();
public:
    AddObjDialog(QWidget *parent = 0);
    ~AddObjDialog();
    void ShowWarning();
public slots:
    void isFilled();

};

#endif // ADDOBJDIALOG_H
