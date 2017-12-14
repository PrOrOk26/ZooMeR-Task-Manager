#ifndef ADDOBJDIALOG_H
#define ADDOBJDIALOG_H

#include <QWidget>
#include <QtWidgets>
#include <QSettings>
#include <QtSql>

class AddObjDialog : public QDialog {
    Q_OBJECT
private:
    QPushButton* pbSave;
    QPushButton* pbCancel;

    //Functions
    void InitializeComponent();
    void SetupLayouts();
    void ConnectSignals();

    void WriteSettings();
    void ReadSettings();

    //Layouts
    QVBoxLayout* mlay;
    QHBoxLayout* layForButton;
    QHBoxLayout* layForDate;
    QHBoxLayout* layForReg;
    QHBoxLayout* layForRem;


    QLabel* lName;
    QLabel* lDate;
    QLabel* lImportance;
    QLabel* lGroup;
    QLabel* lRegular;
    QLabel* lNotification;
    QLabel* lDescription;    

    QLabel* lWarning;

 public:
    QLineEdit* leName;
    QDateEdit* deDate;
    QTimeEdit* teDate;
    QSpinBox* sbImportance;
    QComboBox* cbGroup;
    QSpinBox* sbRegular;
    QSpinBox* sbNotification;
    QPlainTextEdit* teDescription;


    QSettings* settings;

    AddObjDialog(QWidget *parent = 0);
    ~AddObjDialog();
    void ShowWarning();
public slots:
    void isFilled();

};

#endif // ADDOBJDIALOG_H
