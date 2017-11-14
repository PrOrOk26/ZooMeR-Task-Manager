#ifndef ADDOBJDIALOG_H
#define ADDOBJDIALOG_H

#include <QWidget>
#include <QtWidgets>


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
public:
    AddObjDialog(QWidget *parent = 0);
    bool isFilled();
    void ShowWarning();
public slots:


};

#endif // ADDOBJDIALOG_H
